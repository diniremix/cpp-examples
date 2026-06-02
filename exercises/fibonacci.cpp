#include <cstdio>
#include <format>
#include <iostream>
#include <limits>
#include <ostream>
#include <ranges>

int main()
{
    int numero;

    std::cout << "Introduce un valor:";
    std::cin >> numero;

    // Limpiar el buffer de entrada
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (numero <= 0) {
        std::cout << "el valor debe ser un numero positivo mayor a cero" << std::endl;
        std::cout << "saliendo..." << std::endl;
        return 1;
    }

    std::cout << std::format("generando fibonacci de: {}\n", numero);

    int a = 0;
    int b = 0;
    int c = 1;

    std::cout << std::format("{}\n", a + b + c);

    for (int i : std::views::iota(0, numero)) {
        // std::cout << "Número: " << i << std::endl;
        a = b;
        b = c;
        c = a + b;
        std::cout << std::format("{}\n", c);
    }
}
