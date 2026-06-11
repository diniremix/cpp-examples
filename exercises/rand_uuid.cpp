#include <fmt/core.h>
#include <sodium.h>

int main()
{
    if (sodium_init() < 0) {
        return 1;
    }

    fmt::println("Generando números aleatorios uniformes:");

    // Cambiar este valor para definir el rango
    const unsigned int upper_bound = 100;

    for (int i = 0; i < 10; ++i) {
        unsigned int random_number = randombytes_uniform(upper_bound);
        fmt::println("Número: {}: {}", i + 1, random_number);
    }

    return 0;
}
