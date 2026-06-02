#include <fmt/core.h>
#include <format>
#include <iostream>
#include <string>

int main()
{
    int numero = 10;
    std::string texto = "hello C++";

    std::printf("con printf y salto de linea\n");
    std::cout << "usando cout" << std::endl;
    std::cout << std::format("usando format, el numero es: {} y el mensaje es: '{}'\n", numero, texto);
    fmt::println("utilizando ftm::println");

    return 0;
}
