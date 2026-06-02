#include <iostream>
#include <ranges>

bool is_prime(int x)
{
    for (size_t i = 2; i <= x / 2; ++i) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

int main()
{
    std::printf("numeros primos hasta el 200\n");

    // for (size_t i = 1; i <= 200; ++i) {
    for (int i : std::views::iota(1, 200)) {
        if (is_prime(i)) {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";
}
