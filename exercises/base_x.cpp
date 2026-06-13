#include <cassert>
#include <cstddef>
#include <fmt/core.h>
#include <sodium.h>
#include <string>
#include <string_view>

/*
 * se utilizan funciones internas de libsodium
 * para el uso de base64, si se desea usar
 * otros encoding, se recomienda utilizar
 * librerías especificas
 * - cppcodec
 * - Boost
 */

static constexpr char BASE32_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

std::string to_base32(std::string_view input)
{
    const unsigned char* data = reinterpret_cast<const unsigned char*>(input.data());

    std::string output;

    int buffer = 0;
    int bits_left = 0;

    for (size_t i = 0; i < input.size(); ++i) {
        buffer = (buffer << 8) | data[i];
        bits_left += 8;

        while (bits_left >= 5) {
            int index = (buffer >> (bits_left - 5)) & 0x1F;
            output.push_back(BASE32_ALPHABET[index]);
            bits_left -= 5;
        }
    }

    if (bits_left > 0) {
        int index = (buffer << (5 - bits_left)) & 0x1F;
        output.push_back(BASE32_ALPHABET[index]);
    }

    return output;
}

int base32_value(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= '2' && c <= '7')
        return c - '2' + 26;
    return -1;
}

std::string from_base32(std::string_view input)
{
    std::string output;

    int buffer = 0;
    int bits_left = 0;

    for (char c : input) {
        int value = base32_value(c);
        if (value < 0)
            continue; // ignora padding o basura

        buffer = (buffer << 5) | value;
        bits_left += 5;

        if (bits_left >= 8) {
            output.push_back(static_cast<char>((buffer >> (bits_left - 8)) & 0xFF));
            bits_left -= 8;
        }
    }

    return output;
}

std::string to_base64(std::string_view input, bool url_safe = false)
{
    int variant = sodium_base64_VARIANT_ORIGINAL;

    if (url_safe) {
        variant = sodium_base64_VARIANT_URLSAFE_NO_PADDING;
    }

    const auto encoded_size = sodium_base64_encoded_len(input.size(), variant);

    std::string output(encoded_size, '\0');

    sodium_bin2base64(output.data(), output.size(), reinterpret_cast<const unsigned char*>(input.data()), input.size(),
                      variant);

    // Restringimos a `encoded_size`
    // output.resize(encoded_size);

    // ó de la forma
    // output.resize(std::strlen(output.c_str()));
    //
    // return output;

    return std::string(output.c_str());
}

std::string from_base64(std::string_view encoded, bool url_safe = false)
{
    int variant = sodium_base64_VARIANT_ORIGINAL;

    if (url_safe) {
        variant = sodium_base64_VARIANT_URLSAFE_NO_PADDING;
    }

    std::string output(encoded.size(), '\0');

    std::size_t written{};

    if (sodium_base642bin(reinterpret_cast<unsigned char*>(output.data()), output.size(), encoded.data(),
                          encoded.size(), nullptr, &written, nullptr, variant) != 0) {
        return {};
    }

    output.resize(written);

    return output;
}

// genera un base64 desde N de bytes
std::string generate_token(std::size_t bytes = 32)
{
    std::string buffer(bytes, '\0');

    randombytes_buf(buffer.data(), bytes);

    const std::size_t encoded_size = sodium_base64_encoded_len(bytes, sodium_base64_VARIANT_URLSAFE_NO_PADDING);

    std::string encoded(encoded_size, '\0');

    sodium_bin2base64(encoded.data(), encoded.size(), reinterpret_cast<const unsigned char*>(buffer.data()),
                      buffer.size(), sodium_base64_VARIANT_URLSAFE_NO_PADDING);

    encoded.resize(std::strlen(encoded.c_str()));

    return encoded;
}

int main()
{
    if (sodium_init() < 0) {
        return 1;
    }

    fmt::println("BaseXX examples");

    std::string data = "El veloz murciélago hindú comía feliz cardillo y kiwi. La cigüeña ¡tocaba el saxofón detrás "
                       "del palenque de paja!.";

    // base32
    auto encoded = to_base32(data);
    fmt::println("base32: '{}'", encoded);
    auto decoded = from_base32(encoded);
    fmt::println("from_base32: '{}'", decoded);
    fmt::println("size: {}", encoded.size());

    // base64
    encoded = to_base64(data, false);
    fmt::println("base64: '{}'", encoded);
    auto result = from_base64(encoded, false);
    fmt::println("from_base64: '{}'", result);
    fmt::println("size: {}", encoded.size());

    assert(data == result);

    fmt::println("");

    data = "https://api.example.com/v1/"
           "search?q=¡Hola!%20¿Cómo%20estás?%20Café%20☕%20🚀%20中文%20日本語%20한국어";
    encoded = to_base64(data, true);

    fmt::println("base64: '{}'", encoded);
    fmt::println("size: {}", encoded.size());
    result = from_base64(encoded, true);
    fmt::println("from_base64: '{}'", result);

    assert(data == result);

    fmt::println("");

    data = "https://api.example.com/v1/"
           "search?q=C%2B%2B20%20%26%20C%2B%2B23%20-%20SHA512%20%2F%20BLAKE3%20%2F%20Base64%20URL-safe&user=helio.dev%"
           "2Btest@example.com&redirect=https%3A%2F%2Fsub.example.org%3A8443%2Fcallback%3Fa%3D1%26b%3D2%26c%3Dhello%"
           "2520world&symbols=%21%40%23%24%25%5E%26%2A%28%29-%3D_%2B%5B%5D%7B%7D%3B%27%3A%22%2C.%3C%3E%2F%3F#hash-"
           "fragment";
    encoded = to_base64(data, true);

    fmt::println("base64: '{}'", encoded);
    fmt::println("size: {}", encoded.size());
    result = from_base64(encoded, true);
    fmt::println("from_base64: '{}'", result);

    assert(data == result);

    fmt::println("");

    fmt::println("tokens");
    result = generate_token();
    fmt::println("token(32 bytes):'{}'", result);
    result = generate_token(64);
    fmt::println("token(64 bytes):'{}'", result);

    return 0;
}
