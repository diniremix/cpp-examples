#include <cstdio>
#include <format>
#include <iostream>
#include <limits>
#include <ostream>

long factorial(long a)
{
    if (a == 1) {
        return 1;
    }
    return a * factorial(a - 1);
}

int main()
{
    int numero;

    std::printf("factorial de un numero\n");
    std::cout << "Introduce un número entero (no muy grande)  ;-) :";
    std::cin >> numero;

    // Limpiar el buffer de entrada
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (numero <= 0) {
        std::cout << "el valor debe ser un numero positivo mayor a cero" << std::endl;
        std::cout << "saliendo..." << std::endl;
        return 1;
    }

    long result = factorial(numero);
    std::cout << std::format("el factorial de {} es: {}\n", numero, result);
}
