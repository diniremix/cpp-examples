/*
 * Este ejercicio implementa una capa de JWT estricta, inspirada en
 * PASETO v4, reduciendo la flexibilidad peligrosa de JWT y
 * forzando un contrato explícito de autenticación.
 * librerías utilizadas:
 * - jwt-cpp
 * - picojson
 * - sodium (uuid)
 * - dotenv
 *
 * algoritmo(firma/verificación): Ed25519
 * validación estricta de los campos:
 * - 1. issuer(iss): definido explicitamente
 * - 2. audience(aud): definido explicitamente
 * - 3. expiration(exp)
 * - 4. not before(nbf)
 * - 5. issued at(iat) validando anti clock-skew/ token futuro
 * - 6. ctx (implicit assertion equivalente a PASETO V4)
 * - 7. jti: UUID v4 (con prefijo adicional)
 * - 8. sub: campo obligatorio
 * - 9. data: estructura tipada del payload "data", campo obligatorio
 *
 * las claves privadas y públicas, se generan con openSSL y se
 * cargan desde archivos `.pem`
 * - generar llave privada:
 * `openssl genpkey -algorithm ed25519 -out private_key.pem`
 * - generar llave pública:
 * `openssl pkey -in private_key.pem -pubout -out public_key.pem`
 *
 * variables de entorno utilizadas:
 * - `JWT_PRIVATE_KEY_FILE=/path/to/private_key.pem`
 * - `JWT_PUBLIC_KEY_FILE=/path/to/public_key.pem`
 * - `JWT_KEY_TOKEN=awesome_jwt_token_to_validate`
 */

#include "dotenv.hpp"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fmt/core.h>
#include <fstream> // Para ifstream, ofstream, fstream
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/kazuho-picojson/defaults.h>

#if __has_include(<picojson.h>)
    #pragma message("picojson from xmake")
    #include <picojson.h>
#else
    #include <picojson/picojson.h>
#endif

#include <sodium.h>
#include <stdexcept>
#include <string>

using namespace std;
namespace fs = std::filesystem;

struct UserData {
    int user_id;
    std::string user_name;
    int role_id;
    std::string role_name;
    int company_id;
    std::string company_name;
};

UserData parse_user_data(const picojson::value& v)
{
    if (!v.is<picojson::object>()) {
        throw std::runtime_error("claim_data must be object");
    }

    const auto& obj = v.get<picojson::object>();

    auto get_str = [&](const std::string& key) -> std::string {
        auto it = obj.find(key);
        if (it == obj.end() || !it->second.is<std::string>()) {
            throw std::runtime_error("claim_data missing or invalid field: " + key);
        }
        return it->second.get<std::string>();
    };

    auto get_int = [&](const std::string& key) -> int {
        auto it = obj.find(key);
        if (it == obj.end() || !it->second.is<double>()) {
            throw std::runtime_error("claim_data missing or invalid field: " + key);
        }
        return static_cast<int>(it->second.get<double>());
    };

    return UserData{.user_id = get_int("user_id"),
                    .user_name = get_str("user_name"),
                    .role_id = get_int("role_id"),
                    .role_name = get_str("role_name"),
                    .company_id = get_int("company_id"),
                    .company_name = get_str("company_name")};
}

void validate_user_data(const UserData& d)
{
    if (d.user_id <= 0)
        throw std::runtime_error("validata claim_data: invalid user_id");
    if (d.role_id <= 0)
        throw std::runtime_error("validata claim_data: invalid role_id");
    if (d.company_id <= 0)
        throw std::runtime_error("validata claim_data: invalid company_id");

    if (d.user_name.empty())
        throw std::runtime_error("validata claim_data: empty user_name");
    if (d.role_name.empty())
        throw std::runtime_error("validata claim_data: empty role_name");
}

picojson::value to_json(const UserData& d)
{
    picojson::object obj;

    obj["user_id"] = picojson::value(static_cast<double>(d.user_id));
    obj["user_name"] = picojson::value(d.user_name);

    obj["role_id"] = picojson::value(static_cast<double>(d.role_id));
    obj["role_name"] = picojson::value(d.role_name);

    obj["company_id"] = picojson::value(static_cast<double>(d.company_id));
    obj["company_name"] = picojson::value(d.company_name);

    return picojson::value(obj);
}

// =========
std::string read_file(const fs::path& path)
{
    fmt::println("loading: '{}'", path.string());
    std::ifstream file(path);

    if (!file) {
        auto msg = fmt::format("cannot open file: '{}'", path.string());
        throw std::runtime_error(msg);
    }

    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

static std::string load_env(std::string_view key)
{
    const char* v = std::getenv(key.data());
    if (!v) {
        throw std::runtime_error("missing env: " + std::string(key));
    }
    return v;
}

namespace naive_uuid {

    uint64_t unix_time_ms()
    {

        return static_cast<uint64_t>(
            chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
    }

    std::string print_v4(const std::array<unsigned char, 16>& id)
    {
        return fmt::format("{:02x}{:02x}{:02x}{:02x}-"
                           "{:02x}{:02x}-"
                           "{:02x}{:02x}-"
                           "{:02x}{:02x}-"
                           "{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
                           id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8], id[9], id[10], id[11], id[12],
                           id[13], id[14], id[15]);
    }

    std::string uuid_v4()
    {
        std::array<unsigned char, 16> id;

        randombytes_buf(id.data(), id.size());

        // version 4 -> xxxx0100
        id[6] = (id[6] & 0x0F) | 0x40;

        // variant RFC 4122 -> 10xxxxxx
        id[8] = (id[8] & 0x3F) | 0x80;

        return print_v4(id);
    }

} // namespace naive_uuid

namespace token_service {
    inline constexpr std::string_view IMPLICIT_ASSERT = "api.dione.org";
    inline constexpr std::string_view ISSUER = "Dione Api";
    inline constexpr std::string_view AUDIENCE = "https://example.com/dione";
    inline constexpr std::string_view TOKEN_TYPE = "v4.public.";

    struct JwtConfig {
        std::string private_key;
        std::string public_key;
    };

    struct ClaimsData {
        std::string sub;
        std::string jti;

        chrono::system_clock::time_point iat;
        chrono::system_clock::time_point nbf;
        chrono::system_clock::time_point exp;

        picojson::value data;
    };

    class JwtService
    {
      public:
        explicit JwtService(const JwtConfig& config) : cfg(config) {}

        std::string generate(const ClaimsData& c)
        {
            return jwt::create()
                .set_issuer(std::string(ISSUER))
                .set_audience(std::string(AUDIENCE))
                .set_subject(c.sub)
                .set_id(c.jti)
                .set_issued_at(c.iat)
                .set_not_before(c.nbf)
                .set_expires_at(c.exp)
                .set_payload_claim("ctx", jwt::claim(std::string(IMPLICIT_ASSERT)))
                .set_payload_claim("data", jwt::claim(c.data))
                .sign(jwt::algorithm::ed25519{cfg.public_key, cfg.private_key});
        }

        void verify(const std::string& token)
        {
            /*
                jwt::decoded_jwt<jwt::traits::kazuho_picojson> decoded;

                try {
                    decoded = jwt::verify()
                        .allow_algorithm(jwt::algorithm::ed25519{cfg.public_key})
                        .verify(jwt::decode(token));
                }
                catch (const std::exception& e) {
                    throw std::runtime_error(std::string("invalid signature: ") + e.what());
                }
             */

            auto decoded = jwt::decode(token);

            try {
                jwt::verify()
                    .allow_algorithm(jwt::algorithm::ed25519{cfg.public_key})
                    .with_issuer(std::string(ISSUER))
                    .with_audience(std::string(AUDIENCE))
                    .verify(decoded);
            } catch (const std::exception& e) {
                auto error = fmt::format("verify: invalid signature: {}", e.what());
                throw std::runtime_error(error);
            }

            validate_standard_claims(decoded);
        }

        void validate_standard_claims(const jwt::decoded_jwt<jwt::traits::kazuho_picojson>& decoded)
        {
            const auto now = chrono::system_clock::now();

            // 1. ISSUER
            if (decoded.get_issuer() != std::string(ISSUER)) {
                throw std::runtime_error("invalid issuer");
            }

            // 2. AUDIENCE (puede ser una lista)
            {
                const auto aud = decoded.get_audience();
                bool found = false;

                for (const auto& a : aud) {
                    if (a == std::string(AUDIENCE)) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    throw std::runtime_error("invalid audience");
                }
            }

            // 3. EXPIRATION
            if (decoded.get_expires_at() <= now) {
                throw std::runtime_error("token expired");
            }

            // 4. NOT BEFORE
            if (decoded.get_not_before() > now) {
                throw std::runtime_error("token not active yet");
            }

            // 5. ISSUED AT (anti clock-skew / token futuro)
            if (decoded.get_issued_at() > now + std::chrono::minutes(3)) {
                throw std::runtime_error("invalid iat");
            }

            // 6. CTX (implicit assertion equivalente a PASETO)
            if (!decoded.has_payload_claim("ctx")) {
                throw std::runtime_error("missing ctx");
            }

            try {
                if (decoded.get_payload_claim("ctx").as_string() != std::string(IMPLICIT_ASSERT)) {
                    throw std::runtime_error("invalid ctx(IMPLICIT_ASSERT)");
                }
            } catch (const std::exception& e) {
                auto error = fmt::format("invalid CTX: {}", e.what());
                throw std::runtime_error(error);
            }

            // 7. JTI
            if (!decoded.has_payload_claim("jti")) {
                throw std::runtime_error("missing jti");
            }

            try {
                if (decoded.get_payload_claim("jti").as_string().empty()) {
                    throw std::runtime_error("invalid jti(str)");
                }
            } catch (const std::exception& e) {
                auto error = fmt::format("invalid JTI: {}", e.what());
                throw std::runtime_error(error);
            }

            // 8. SUB obligatorio
            if (!decoded.has_payload_claim("sub")) {
                throw std::runtime_error("missing sub");
            }

            try {
                if (decoded.get_payload_claim("sub").as_string().empty()) {
                    throw std::runtime_error("invalid sub(str)");
                }
            } catch (const std::exception& e) {
                auto error = fmt::format("invalid SUB: {}", e.what());
                throw std::runtime_error(error);
            }

            // 9. DATA obligatorio
            if (!decoded.has_payload_claim("data")) {
                throw std::runtime_error("missing data");
            }

            try {
                auto data_claim = decoded.get_payload_claim("data");

                UserData data = parse_user_data(data_claim.to_json());
                validate_user_data(data);
            } catch (const std::exception& e) {
                auto error = fmt::format("invalid data: {}", e.what());
                throw std::runtime_error(error);
            }
        }

      private:
        JwtConfig cfg;
    };
} // namespace token_service

int main()
{
    fmt::println("JWT example");
    fmt::println("");

    dotenv::load(".env");

    auto private_key = read_file(load_env("JWT_PRIVATE_KEY_FILE"));
    auto public_key = read_file(load_env("JWT_PUBLIC_KEY_FILE"));

    token_service::JwtConfig cfg(private_key, public_key);
    token_service::JwtService service(cfg);

    auto username = "john_connor";

    UserData data{.user_id = 1,
                  .user_name = username,
                  .role_id = 2,
                  .role_name = "ADMIN",
                  .company_id = 3,
                  .company_name = "Home Inside Inc."};

    auto json_data = to_json(data);

    auto uidv4 = naive_uuid::uuid_v4();

    token_service::ClaimsData c{.sub = username,
                                .jti = fmt::format("rt_{}", uidv4),
                                .iat = chrono::system_clock::now(),
                                .nbf = chrono::system_clock::now(),
                                .exp = chrono::system_clock::now() + chrono::minutes(15),
                                .data = json_data};

    auto token = service.generate(c);
    fmt::println("");

    fmt::println("new TOKEN: {}", token);
    fmt::println("");

    service.verify(token);
    fmt::println("validation OK");
    fmt::println("");

    fmt::println("==========Validate (JWT_KEY_TOKEN)==========");
    fmt::println("");
    auto token_key = load_env("JWT_KEY_TOKEN");

    fmt::println("token_key to validate: '{}'", token_key);

    service.verify(token_key);
    fmt::println("validation OK");
}
