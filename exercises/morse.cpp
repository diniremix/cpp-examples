#include <algorithm>
#include <cctype>
#include <expected>
#include <fmt/core.h>
#include <format>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <uni_algo/all.h>
#include <unordered_map>
#include <vector>

namespace utils {
    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, std::string_view>
    std::string join(const R& range, std::string_view sep = ",")
    {
        std::string result;

        auto it = range.begin();

        if (it == range.end())
            return result;

        result += std::string_view{*it++};

        while (it != range.end()) {
            result += sep;
            result += std::string_view{*it++};
        }

        return result;
    }

    std::string to_lower_ascii(std::string s)
    {
        std::ranges::transform(s, s.begin(), [](unsigned char c) { return std::tolower(c); });

        return s;
    }

    std::string to_upper_ascii(std::string s)
    {
        std::ranges::transform(s, s.begin(), [](unsigned char c) { return std::toupper(c); });

        return s;
    }

    std::string trim(std::string_view str)
    {
        auto start = str.find_first_not_of(" \t\n\r\f\v");

        if (start == std::string_view::npos)
            return {};

        auto end = str.find_last_not_of(" \t\n\r\f\v");

        return std::string{str.substr(start, end - start + 1)};
    }

    std::vector<std::string> split(std::string_view text, std::string_view delimiter)
    {
        std::vector<std::string> result;

        if (delimiter.empty()) {
            result.emplace_back(text);
            return result;
        }

        std::size_t start = 0;

        while (true) {
            auto pos = text.find(delimiter, start);

            if (pos == std::string_view::npos) {
                result.emplace_back(text.substr(start));
                break;
            }

            result.emplace_back(text.substr(start, pos - start));

            start = pos + delimiter.size();
        }

        return result;
    }

} // namespace utils

struct MorseEntry {
    char32_t character;
    std::string_view morse;
};

constexpr std::array morse_table{
    MorseEntry{U'a', "._"},
    MorseEntry{U'b', "_..."},
    MorseEntry{U'c', "_._."},
};

int main()
{

    std::string text = "El veloz murciélago hindú comía feliz cardillo y kiwi. La cigüeña ¡tocaba el saxofón detrás "
                       "del palenque de paja!.";
    fmt::println("normal text: '{}'", text);
    // fmt::println("normal text: '{}'", utils::to_upper_ascii(text));
    // fmt::println("normal text: '{}'", utils::to_lower_ascii(text));
    // fmt::println("normal text: '{}'", utils::trim(" , a -- a "));
    /*auto result_split = utils::split("a::b::c", "::");
    for (const auto& token : result_split) {
        fmt::println("{}", token);
    }*/

    /*auto result_enc = encode(text);
    if (!result_enc) {
        fmt::println(stderr, "encode error: {}", result_enc.error());
        return 1;
    } else {
        fmt::println("{}", *result_enc);
    }

    auto result_dec = decode(*result_enc);
    if (!result_dec) {
        fmt::println(stderr, "decode error: {}", result_dec.error());
    } else {
        fmt::println("{}", *result_dec);
    }

    // ejemplos con una::
    //  char to ascii code
    char vowel = 'a';
    auto code = static_cast<std::uint8_t>(vowel);
    fmt::println("vowel: {} -> code: {}", vowel, code);

    char32_t sp_char = U'ñ';
    auto n_vowel = una::utf32to8(std::u32string(1, sp_char));
    code = static_cast<std::uint32_t>(sp_char);
    fmt::println("sp_char: {} -> code: {} -> using 'una::*'", n_vowel, code);*/

    fmt::println("");
    return 0;
} // namespace utils
