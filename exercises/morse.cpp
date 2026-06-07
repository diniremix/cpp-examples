#include <algorithm>
#include <cctype>
#include <expected>
#include <fmt/core.h>
#include <format>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <uni_algo/all.h>
#include <unordered_map>
#include <vector>

namespace unicode {
    std::string to_lower(std::string_view text)
    {
        return una::cases::to_lowercase_utf8(text);
    }

    std::string to_upper(std::string_view text)
    {
        return una::cases::to_uppercase_utf8(text);
    }

    std::string normalize(std::string_view text)
    {
        return una::norm::to_nfc_utf8(text);
    }
    std::u32string to_utf32(std::string_view text)
    {
        // return una::utf8to32(text);

        /*
        std::wstring wide_string = una::utf8to32(text);
        std::u32string utf32_string;

        // Convertir cada wchar_t a char32_t
        for (const auto& wc : wide_string) {
            utf32_string.push_back(static_cast<char32_t>(wc));
        }

        return utf32_string;
        */
        auto wide = una::utf8to32(text);
        std::u32string result(wide.begin(), wide.end());
        return result;
    }
    std::string to_utf8(const std::u32string& text)
    {
        return una::utf32to8(text);
    }
} // namespace unicode

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

namespace morse {

    struct MorseEntry {
        char32_t character;
        std::string_view morse;
    };

    /*
    constexpr std::array morse_table{
        MorseEntry{U'a', "._"},    MorseEntry{U'b', "_..."},  MorseEntry{U'c', "_._."},
        MorseEntry{U'd', "_.."},   MorseEntry{U'e', "."},

        MorseEntry{U'á', ".__._"}, MorseEntry{U'é', ".._.."}, MorseEntry{U'ñ', "__.__"},

        MorseEntry{U'0', "_____"}, MorseEntry{U'1', ".____"},

        MorseEntry{U' ', "/"},
    };*/
    // Definición de la tabla Morse
    constexpr std::array<MorseEntry, 94> morse_table{
        MorseEntry{U'a', "._"},     MorseEntry{U'á', "..__._"},  MorseEntry{U'b', "_..."},
        MorseEntry{U'c', "_._."},   MorseEntry{U'd', "_.."},     MorseEntry{U'e', "."},
        MorseEntry{U'é', ".._.."},  MorseEntry{U'f', ".._."},    MorseEntry{U'g', "__."},
        MorseEntry{U'h', "...."},   MorseEntry{U'i', ".."},      MorseEntry{U'í', ".."},
        MorseEntry{U'j', ".___"},   MorseEntry{U'k', "_._"},     MorseEntry{U'l', "._.."},
        MorseEntry{U'm', "__"},     MorseEntry{U'n', "_."},      MorseEntry{U'ñ', ".__.__"},
        MorseEntry{U'o', "___"},    MorseEntry{U'ó', "__._."},   MorseEntry{U'p', ".__."},
        MorseEntry{U'q', "__._"},   MorseEntry{U'r', "._."},     MorseEntry{U's', "..."},
        MorseEntry{U't', "_"},      MorseEntry{U'u', ".._"},     MorseEntry{U'ú', ".._"},
        MorseEntry{U'v', "..._"},   MorseEntry{U'w', ".__"},     MorseEntry{U'x', "_.._"},
        MorseEntry{U'y', "_.__"},   MorseEntry{U'z', "__.."},    MorseEntry{U'æ', "._._"},
        MorseEntry{U'ø', "__._."},  MorseEntry{U'å', ".__._"},   MorseEntry{U'ü', "..__"},
        MorseEntry{U'0', "_____"},  MorseEntry{U'1', ".____"},   MorseEntry{U'2', "..___"},
        MorseEntry{U'3', "...__"},  MorseEntry{U'4', "...._"},   MorseEntry{U'5', "....."},
        MorseEntry{U'6', "_...."},  MorseEntry{U'7', "__..."},   MorseEntry{U'8', "___.."},
        MorseEntry{U'9', "____."},  MorseEntry{U'.', "._._._"},  MorseEntry{U',', "__..__"},
        MorseEntry{U'¿', ".._._"},  MorseEntry{U'?', "..__.."},  MorseEntry{U'\'', ".____."},
        MorseEntry{U'¡', "__..._"}, MorseEntry{U'!', "_._.__"},  MorseEntry{U'/', "_.._."},
        MorseEntry{U'(', "_.__."},  MorseEntry{U')', "_.__._"},  MorseEntry{U'&', "._..."},
        MorseEntry{U':', "___..."}, MorseEntry{U';', "_._._."},  MorseEntry{U'=', "_..._"},
        MorseEntry{U'+', "._._."},  MorseEntry{U'-', "_...._"},  MorseEntry{U'_', "..__._"},
        MorseEntry{U'"', "._.._."}, MorseEntry{U'$', "..._.._"}, MorseEntry{U'@', ".__._."},
        MorseEntry{U' ', "/"},
    };

    std::string_view get_morse(char32_t ch)
    {
        for (const auto& entry : morse_table) {
            if (entry.character == ch) {
                return entry.morse;
            }
        }

        return "#";
    }

    char32_t get_character(std::string_view morse)
    {
        for (const auto& entry : morse_table) {
            if (entry.morse == morse) {
                return entry.character;
            }
        }

        return U'#';
    }

    std::optional<std::string_view> find_morse(char32_t ch)
    {
        for (const auto& entry : morse_table) {
            if (entry.character == ch) {
                return entry.morse;
            }
        }

        return std::nullopt;
    }

    std::expected<std::string, std::string> encode(std::string_view input)
    {
        auto normalized = unicode::normalize(unicode::to_lower(input));

        auto text32 = unicode::to_utf32(normalized);

        std::vector<std::string> result;
        std::vector<std::string> errors;

        for (char32_t cp : text32) {

            auto morse = find_morse(cp);

            if (!morse) {
                errors.push_back(unicode::to_utf8(std::u32string{cp}));
                continue;
            }

            result.emplace_back(*morse);
        }

        fmt::println("partial converted text: '{}'", utils::join(result, " "));
        fmt::println("");
        if (!errors.empty()) {
            return std::unexpected(std::format("unsupported characters: {}", utils::join(errors)));
        }

        return utils::join(result, " ");
    }

} // namespace morse

int main()
{

    std::string text = "El veloz murciélago hindú comía feliz cardillo y kiwi. La cigüeña ¡tocaba el saxofón detrás "
                       "del palenque de paja!.";
    fmt::println("normal text: '{}'", text);

    auto result_enc = morse::encode(text);
    if (!result_enc) {
        fmt::println(stderr, "encode error: {}", result_enc.error());
        return 1;
    } else {
        fmt::println("morse result: '{}'", *result_enc);
    }

    /*
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
