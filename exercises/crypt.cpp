#include <fmt/core.h>
#include <iostream>
#include <sodium.h>
#include <sstream>
#include <string>
#include <vector>

/*
 * configuraciones para Argon2id
 *
 * - OPSLIMIT (tiempo / CPU)
 * Define cuántas iteraciones internas se ejecutan
 * y aumenta tiempo de cómputo linealmente
 * impactando la CPU directamente
 *
 * - MEMLIMIT (memoria)
 * Define cuánta RAM usa cada hash
 *
 * modos de uso
 *
 * - INTERACTIVE
 * baja latencia, bajo consumo
 * RAM baja (pocos MB a ~10-20 MB según config)
 * CPU muy rápido (decenas de ms)
 *
 * - MODERATE
 * equilibrio seguridad/rendimiento
 * más costoso en RAM y CPU
 *
 * - SENSITIVE
 * RAM alta (cientos de MB por operación posible)
 * CPU muy lento (cientos de ms a segundos)
 *
 * para este ejemplo se utiliza INTERACTIVE con los
 * siguientes ajustes:
 * ```
 * const unsigned long long opslimit = crypto_pwhash_OPSLIMIT_INTERACTIVE;
 * const size_t memlimit = 16 * 1024 * 1024; // 16 MB
 * ```
 */

struct ParsedHash {
    size_t memlimit;
    unsigned long long opslimit;
    std::vector<unsigned char> salt;
    std::vector<unsigned char> hash;
};

ParsedHash parse_hash(const std::string& input)
{
    ParsedHash ph;

    std::string tmp = input;

    // formato:
    // argon2id$v=19$m=...,t=...,p=1$salt$hash

    size_t pos1 = tmp.find("$");
    size_t pos2 = tmp.find("$", pos1 + 1);
    size_t pos3 = tmp.find("$", pos2 + 1);
    size_t pos4 = tmp.find("$", pos3 + 1);

    std::string params = tmp.substr(pos2 + 1, pos3 - pos2 - 1);

    std::string salt_b64 = tmp.substr(pos3 + 1, pos4 - pos3 - 1);
    std::string hash_b64 = tmp.substr(pos4 + 1);

    // parse params
    sscanf(params.c_str(), "m=%zu,t=%llu,p=1", &ph.memlimit, &ph.opslimit);

    // decode base64 salt/hash
    ph.salt.resize(crypto_pwhash_SALTBYTES);
    ph.hash.resize(32);

    sodium_base642bin(ph.salt.data(), ph.salt.size(), salt_b64.c_str(), salt_b64.size(), nullptr, nullptr, nullptr,
                      sodium_base64_VARIANT_ORIGINAL);

    sodium_base642bin(ph.hash.data(), ph.hash.size(), hash_b64.c_str(), hash_b64.size(), nullptr, nullptr, nullptr,
                      sodium_base64_VARIANT_ORIGINAL);

    return ph;
}

bool verify_password(const std::string& password, const ParsedHash& ph)
{
    std::vector<unsigned char> computed(ph.hash.size());

    crypto_pwhash(computed.data(), computed.size(), password.c_str(), password.size(), ph.salt.data(), ph.opslimit,
                  ph.memlimit, crypto_pwhash_ALG_ARGON2ID13);

    return sodium_memcmp(computed.data(), ph.hash.data(), ph.hash.size()) == 0;
}

static std::string b64(const std::vector<unsigned char>& data)
{
    std::string out;
    out.resize(sodium_base64_ENCODED_LEN(data.size(), sodium_base64_VARIANT_ORIGINAL));

    sodium_bin2base64(out.data(), out.size(), data.data(), data.size(), sodium_base64_VARIANT_ORIGINAL);

    if (!out.empty() && out.back() == '\0')
        out.pop_back();

    return out;
}

std::string hash_password(const std::string& password)
{
    std::vector<unsigned char> salt(crypto_pwhash_SALTBYTES);
    randombytes_buf(salt.data(), salt.size());

    std::vector<unsigned char> hash(32);

    // const unsigned long long opslimit = crypto_pwhash_OPSLIMIT_MODERATE;
    //  const size_t memlimit = crypto_pwhash_MEMLIMIT_MODERATE;

    // const unsigned long long opslimit = crypto_pwhash_OPSLIMIT_INTERACTIVE;
    // const size_t memlimit = crypto_pwhash_MEMLIMIT_INTERACTIVE;

    const unsigned long long opslimit = crypto_pwhash_OPSLIMIT_INTERACTIVE;
    const size_t memlimit = 16 * 1024 * 1024; // 16 MB

    crypto_pwhash(hash.data(), hash.size(), password.c_str(), password.size(), salt.data(), opslimit, memlimit,
                  crypto_pwhash_ALG_ARGON2ID13);

    std::ostringstream oss;

    oss << "argon2id$v=19" << "$m=" << memlimit << ",t=" << opslimit << ",p=1" << "$" << b64(salt) << "$" << b64(hash);

    return oss.str();
}

int main()
{
    if (sodium_init() < 0)
        return 1;

    std::string password = "mi_password_segura";

    // REGISTER
    std::string stored = hash_password(password);

    // guardar en DB
    fmt::println("Stored: {}", stored);

    // LOGIN
    ParsedHash ph = parse_hash(stored);

    bool ok = verify_password("mi_password_segura", ph);

    if (ok)
        fmt::println("LOGIN OK");
    else
        fmt::println("LOGIN FAIL");
}
