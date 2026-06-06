#include <fmt/core.h>
#include <format>
#include <iostream>
#include <string>
#ifdef __cpp_lib_print
    #include <print>
#endif

int main()
{
    int numero = 10;
    std::string texto = "hello C++";

    std::printf("con printf y salto de linea\n");
    std::cout << "usando cout" << std::endl;
    std::cout << std::format("usando format, el numero es: {} y el mensaje es: '{}'\n", numero, texto);
    fmt::println("utilizando ftm::println");

#ifdef __cpp_lib_print
    std::println("Usa print");
    std::print("el texto es: {}\n", texto);
#endif

    return 0;
}
