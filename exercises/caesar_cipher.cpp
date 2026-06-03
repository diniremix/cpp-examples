#include <fmt/core.h>
#include <string>

std::string rot13(std::string text)
{
    int shift = 13;
    int alpha_chars = 26;
    std::string result;

    for (auto it = text.begin(); it != text.end(); ++it) {
        // fmt::println("{}", *it);

        auto code = static_cast<std::uint32_t>(*it);
        // fmt::print("code: {}", code);

        // a-z
        if (code >= 97 && code <= 122) {
            int ord = (code - 97) + shift;
            ord = (ord % alpha_chars) + 97;

            // result += ord;
            result += static_cast<char>(ord);
        }
        // A-Z
        else if (code >= 65 && code <= 90) {
            int ord = (code - 65) + shift;
            ord = (ord % alpha_chars) + 65;

            // result += ord;
            result += static_cast<char>(ord);
        } else {
            result += *it;
        }
    }

    return result;
}

int main()
{
    std::string text = "nowhere ABJURER";
    std::string result_chiper = rot13(text);

    fmt::println("cipher text: {}", result_chiper);
    std::string result_text = rot13(result_chiper);
    fmt::println("original text: {}", result_text);

    fmt::println("");

    // The Wikipedia example uses a right offset of 23
    // instead of the classic 13.
    text = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG";
    result_chiper = rot13(text);
    fmt::println("cipher text: {}", result_chiper);

    result_text = rot13(result_chiper);
    fmt::println("original text: {}", result_text);

    return 0;
}
