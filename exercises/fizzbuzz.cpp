#include <fmt/core.h>

int main()
{
    size_t n = 1;

    fmt::println("fizzbuzz");
    fmt::println("");

    for (size_t i = 1; n < 101; ++i) {
        if (n % 15 == 0) {
            fmt::println("fizzbuzz");
        } else if (n % 3 == 0) {
            fmt::println("fizz");
        } else if (n % 5 == 0) {
            fmt::println("buzz");
        } else {
            fmt::println("{}", n);
        }
        n += 1;
    }

    return 0;
}
