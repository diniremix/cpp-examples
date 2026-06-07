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

namespace morse {
    char getCharacterEquivalent(const std::string& morse)
    {
        static const std::unordered_map<std::string, char> charMap = {
            {"._", 'a'},      {".__._", 'á'},  {"_...", 'b'},  {"_._.", 'c'},   {"_..", 'd'},     {".", 'e'},
            {".._..", 'é'},   {".._.", 'f'},   {"__.", 'g'},   {"....", 'h'},   {"..", 'i'},      {".___", 'j'},
            {"_._", 'k'},     {"._..", 'l'},   {"__", 'm'},    {"_.", 'n'},     {"__.__", 'ñ'},   {"___", 'o'},
            {"___.", 'ó'},    {".__.", 'p'},   {"__._", 'q'},  {"._.", 'r'},    {"...", 's'},     {"_", 't'},
            {".._", 'u'},     {"..._", 'v'},   {".__", 'w'},   {"_.._", 'x'},   {"_.__", 'y'},    {"__..", 'z'},
            {"._._", 'æ'},    {"..__", 'ü'},   {"_____", '0'}, {".____", '1'},  {"..___", '2'},   {"...__", '3'},
            {"...._", '4'},   {".....", '5'},  {"_....", '6'}, {"__...", '7'},  {"___..", '8'},   {"____.", '9'},
            {"._._._", '.'},  {"__..__", ','}, {".._._", '¿'}, {"..__..", '?'}, {".____.", '\''}, {"__..._", '¡'},
            {"_._.__", '!'},  {"_.._.", '/'},  {"_.__.", '('}, {"_.__._", ')'}, {"._...", '&'},   {"___...", ':'},
            {"_._._.", ';'},  {"_..._", '='},  {"._._.", '+'}, {"_...._", '-'}, {"..__._", '_'},  {"._.._.", '"'},
            {"..._.._", '$'}, {".__._.", '@'}, {" ", ' '}};

        auto it = charMap.find(morse);
        if (it != charMap.end()) {
            return it->second;
        }

        // Manejo de errores
        std::cerr << "Error: código Morse desconocido '" << morse << "'\n";
        return '#';
    }

    std::string getMorseEquivalent(char c)
    {
        static const std::unordered_map<char, std::string> morseMap = {
            {'a', "._"},    {'á', ".__._"},  {'b', "_..."},    {'c', "_._."},   {'d', "_.."},     {'e', "."},
            {'é', ".._.."}, {'f', ".._."},   {'g', "__."},     {'h', "...."},   {'i', ".."},      {'í', ".."},
            {'j', ".___"},  {'k', "_._"},    {'l', "._.."},    {'m', "__"},     {'n', "_."},      {'ñ', "__._."},
            {'o', "___"},   {'ó', "___.|"},  {'p', ".__."},    {'q', "__._"},   {'r', "._."},     {'s', "..."},
            {'t', "_"},     {'u', ".._"},    {'ú', ".._"},     {'v', "..._"},   {'w', ".__"},     {'x', "_.._"},
            {'y', "_.__"},  {'z', "__.."},   {'æ', "._._"},    {'ø', ".___."},  {'å', ".__._"},   {'ü', "..__"},
            {'0', "_____"}, {'1', ".____"},  {'2', "..___"},   {'3', "...__"},  {'4', "...._"},   {'5', "....."},
            {'6', "_...."}, {'7', "__..."},  {'8', "___.."},   {'9', "____."},  {'.', "._._._"},  {',', "__..__"},
            {'¿', ".._._"}, {'?', "..__.."}, {'\'', ".____."}, {'¡', "__..._"}, {'!', "_._.__"},  {'/', "_.._."},
            {'(', "_.__."}, {')', "_.__._"}, {'&', "._.."},    {':', "___..."}, {';', "_._._."},  {'=', "_..._"},
            {'+', "._._."}, {'-', "_...._"}, {'_', "..__._"},  {'"', "._.._."}, {'$', "..._.._"}, {'@', ".__._"},
            {' ', "/"} // Espacio
        };

        auto it = morseMap.find(c);
        if (it != morseMap.end()) {
            return it->second;
        }

        // Manejo de errores
        std::cerr << "Error: caracter desconocido '" << c << "'\n";
        return "#";
    }
} // namespace morse

namespace utils {

    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, std::string_view>
    std::string join(const R& range, std::string_view sep = ",")
    {
        std::string result;

        auto it = range.begin();

        if (it == range.end())
            return result;

        result += std::string_view(*it++);

        while (it != range.end()) {
            result += sep;
            result += std::string_view(*it++);
        }

        return result;
    }

    std::string to_lower(std::string s)
    {
        std::ranges::transform(s, s.begin(), [](unsigned char c) { return std::tolower(c); });

        return s;
    }

    std::string to_upper(std::string s)
    {
        std::ranges::transform(s, s.begin(), [](unsigned char c) { return std::toupper(c); });

        return s;
    }

    std::vector<std::string> split(std::string str, std::string delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;

        std::stringstream ss(str);
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        return tokens;
    }

    std::string trim(const std::string& str)
    {
        size_t start = str.find_first_not_of(" \t\n\r\f\v"); // Encuentra el primer carácter no blanco
        size_t end = str.find_last_not_of(" \t\n\r\f\v");    // Encuentra el último carácter no blanco

        if (start == std::string::npos)
            return "";                             // Si solo hay espacios
        return str.substr(start, end - start + 1); // Devuelve la subcadena recortada
    }

}; // namespace utils

std::expected<std::string, std::string> encode(std::string input)
{
    std::string text = utils::to_lower(input);
    std::vector<std::string> result = {};
    std::vector<std::string> error_values = {};
    std::string code;
    input = utils::to_lower(input);
    fmt::println("encode input(lower): {}", input);

    for (auto const& c : input) {
        // fmt::println("char: {}", cp);
        auto code = static_cast<std::uint8_t>(c);
        std::string morse = morse::getMorseEquivalent(code);
        result.push_back(morse);
        result.push_back(" ");
    }

    result.pop_back();
    if (error_values.size() > 0) {
        auto msg = std::format("unsupported characters: {}", utils::join(error_values));
        fmt::println(stderr, "{}", msg);
        return std::unexpected(msg);
    }

    return utils::join(result);
}

std::expected<std::string, std::string> decode(std::string input)
{
    fmt::println("decode input: {}", input);
    std::string text = std::regex_replace(input, std::regex("*"), ".");
    fmt::println("decode text: {}", text);
    text = std::regex_replace(text, std::regex("-"), "_");
    text = utils::trim(text);
    fmt::println("decode text: {}", text);
    std::vector<std::string> words = utils::split(text, "/");

    std::vector<std::string> result = {};
    std::vector<std::string> error_values = {};

    for (auto const& c : input) {
        // fmt::println("char: {}", cp);
        auto code = static_cast<std::uint8_t>(c);
        std::string morse = morse::getMorseEquivalent(code);
        result.push_back(morse);
        result.push_back(" ");
    }

    result.pop_back();
    if (error_values.size() > 0) {
        auto msg = std::format("unsupported characters: {}", utils::join(error_values));
        fmt::println(stderr, "{}", msg);
        return std::unexpected(msg);
    }

    return utils::join(result);
}

int main()
{

    std::string text = "El veloz murciélago hindú comía feliz cardillo y kiwi. La cigüeña ¡tocaba el saxofón detrás "
                       "del palenque de paja!.";
    fmt::println("normal text: '{}'", text);

    auto result_enc = encode(text);
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
    fmt::println("sp_char: {} -> code: {} -> using 'una::*'", n_vowel, code);

    fmt::println("");
    return 0;
}
