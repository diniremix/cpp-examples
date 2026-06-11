#include <array>
#include <blake3.h>
#include <fmt/core.h>

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

    return 0;
}
