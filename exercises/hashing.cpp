#include <array>
#include <blake3.h>
#include <fmt/core.h>
#include <sodium.h>
#include <string>

// sha256
std::string sha256_hex(const std::string& input)
{
    std::array<unsigned char, crypto_hash_sha256_BYTES> digest{};

    crypto_hash_sha256(digest.data(), reinterpret_cast<const unsigned char*>(input.data()), input.size());

    char hex[crypto_hash_sha256_BYTES * 2 + 1];

    sodium_bin2hex(hex, sizeof(hex), digest.data(), digest.size());

    return std::string(hex);
}

// sha512
std::string sha512_hex(std::string& input)
{
    std::array<unsigned char, crypto_hash_sha512_BYTES> digest{};

    crypto_hash_sha512(digest.data(), reinterpret_cast<const unsigned char*>(input.data()), input.size());

    std::array<char, crypto_hash_sha512_BYTES * 2 + 1> hex{};

    sodium_bin2hex(hex.data(), hex.size(), digest.data(), digest.size());

    return hex.data();
}

// hash con blake2 (recomendado)
std::string blake2b_32(const std::string& input)
{
    std::array<unsigned char, crypto_generichash_BYTES_MAX> hash{};

    crypto_generichash(hash.data(), hash.size(), reinterpret_cast<const unsigned char*>(input.data()), input.size(),
                       nullptr, 0);

    char hex[crypto_generichash_BYTES_MAX * 2 + 1];

    sodium_bin2hex(hex, sizeof(hex), hash.data(), hash.size());

    return std::string(hex);
}

// utilizando modo 'stream'
namespace blake_stream {
    struct Stream {
        blake3_hasher hasher;
        std::array<std::uint8_t, 32> bytes;

        Stream()
        {
            blake3_hasher_init(&hasher);
        }

        void write(std::string_view data)
        {
            blake3_hasher_update(&hasher, data.data(), data.size());
        }

        void finalize()
        {
            std::array<std::uint8_t, 32> out{};
            blake3_hasher_finalize(&hasher, out.data(), out.size());
            bytes = out;
        }

        std::string bytes_to_hex()
        {
            static constexpr char hex[] = "0123456789abcdef";

            std::string out;
            out.resize(bytes.size() * 2);

            for (size_t i = 0; i < bytes.size(); ++i) {
                auto b = bytes[i];
                out[i * 2] = hex[b >> 4];
                out[i * 2 + 1] = hex[b & 0x0F];
            }

            return out;
        }
    };
} // namespace blake_stream

// utilizando modo 'normal'
namespace blake {
    std::string bytes_to_hex(const std::array<std::uint8_t, 32>& bytes)
    {
        static constexpr char hex[] = "0123456789abcdef";

        std::string out;
        out.resize(bytes.size() * 2);

        for (size_t i = 0; i < bytes.size(); ++i) {
            auto b = bytes[i];
            out[i * 2] = hex[b >> 4];
            out[i * 2 + 1] = hex[b & 0x0F];
        }

        return out;
    }

    std::array<uint8_t, 32> blake3_hash(const std::string& data)
    {
        blake3_hasher hasher;
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, data.data(), data.size());

        // std::vector<uint8_t> hash(BLAKE3_OUT_LEN);
        std::array<std::uint8_t, 32> out{};
        blake3_hasher_finalize(&hasher, out.data(), out.size());

        // return hash;
        return out;
    }
} // namespace blake

int main()
{
    fmt::println("hashing examples");

    std::string data = "The quick brown fox jumps over the lazy dog";

    fmt::println("blake3 (normal mode)");

    auto blake_result = blake::blake3_hash(data);
    /*
    for (auto b : blake_result) {
        fmt::print("{:02x}", b);
    }
    */
    fmt::println("BLAKE3: {}", blake::bytes_to_hex(blake_result));

    fmt::println("");

    fmt::println("blake3 (stream mode)");

    // para que la salida de un string normal (`data`),
    // sea igual al modo stream, hay que agregar espacios
    // entre cada `write`
    blake_stream::Stream h;
    h.write("The");
    h.write(" ");
    h.write("quick");
    h.write(" ");
    h.write("brown");
    h.write(" ");
    h.write("fox");
    h.write(" ");
    h.write("jumps");
    h.write(" ");
    h.write("over");
    h.write(" ");
    h.write("the");
    h.write(" ");
    h.write("lazy");
    h.write(" ");
    h.write("dog");
    h.finalize();

    fmt::println("BLAKE3: {}", h.bytes_to_hex());

    fmt::println("");

    auto result = sha256_hex(data);
    fmt::println("sha256 (sodium): {}", result);
    fmt::println("");

    result = sha512_hex(data);
    fmt::println("sha512 (sodium): {}", result);
    fmt::println("");

    result = blake2b_32(data);
    fmt::println("hashing with blake2 (sodium): {}", result);

    return 0;
}
