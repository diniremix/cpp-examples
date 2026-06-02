#include <cstdio>
#include <format>
#include <iostream>
#include <limits>
#include <ostream>
#include <ranges>

int fibonacci(int num)
{
    if (num < 2) {
        return num;
    }
    return fibonacci(num - 1) + fibonacci(num - 2);
}

int main()
{
    int numero;

    std::printf("calcular numeros de fibonacci\n");
    std::cout << "Introduce un valor:";
    std::cin >> numero;

    // Limpiar el buffer de entrada
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (numero <= 0) {
        std::cout << "el valor debe ser un numero positivo mayor a cero" << std::endl;
        std::cout << "saliendo..." << std::endl;
        return 1;
    }

    int result = fibonacci(numero);
    std::cout << std::format("el fibonacci de {} es: {}\n", numero, result);
}
