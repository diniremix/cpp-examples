#include <fmt/core.h>
#include <string>
#include <uni_algo/all.h>

int main()
{
    // ascii code to char
    char32_t from_rune = 97; // 'a'
    std::uint8_t from_u8 = 97;

    fmt::println("from_rune(char32_t/una): {}", una::utf32to8(std::u32string(1, from_rune)));
    fmt::println("from_u8(uint8_t/static_cast): {}", static_cast<char>(from_u8));

    fmt::println("");

    //  char to ascii code
    char vowel = 'a';
    auto code = static_cast<std::uint8_t>(vowel);
    fmt::println("vowel: {} -> code: {}", vowel, code);

    // aquí hay problemas de unicode
    vowel = 'ñ';
    code = static_cast<std::uint8_t>(vowel);
    fmt::println("sp_char: {} -> code: {} -> using 'std::uint8_t'", vowel, code);

    // con `una::*`
    char32_t sp_char = U'ñ';
    auto n_vowel = una::utf32to8(std::u32string(1, sp_char));
    code = static_cast<std::uint32_t>(sp_char);

    fmt::println("sp_char: {} -> code: {} -> using 'una::*'", n_vowel, code);

    fmt::println("");

    // emojis
    fmt::println("😆😊😎🥳👾😈🤡💀💩🙊🙉🙈💯💥");
    fmt::println("'El veloz murciélago hindú comía feliz cardillo y kiwi. La cigüeña ¡tocaba el saxofón detrás del "
                 "palenque de paja!.'");
    fmt::println("");

    std::u32string text = U"😆😊😎🥳👾😈🤡💀💩🙊🙉🙈💯💥";
    // error al imprimir
    // fmt::println("{}", text);

    for (char32_t cp : text) {
        auto utf8_char = una::utf32to8(std::u32string(1, cp));

        fmt::println("char: {}, code: {} -> {}", utf8_char, static_cast<std::uint32_t>(cp), utf8_char);
    }
}
