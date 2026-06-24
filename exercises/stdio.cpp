#include <cstdio>
#include <fmt/core.h>
#include <iostream>
#include <string>

#ifdef _WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif

bool stdin_is_redirected()
{
#ifdef _WIN32
    return !_isatty(_fileno(stdin));
#else
    return !isatty(STDIN_FILENO);
#endif
}

/*
usando getline
maneja líneas largas automáticamente
más idiomático en c++ moderno
menos riesgo de errores
*/
void get_line()
{
    std::string line;
    if (!line.empty()) {
    }

    while (std::getline(std::cin, line)) {
        fmt::println("{}", line);
    }
}

/*
usando un buffer de tamaño fijo
sin asignaciones dinámicas
debes elegir un tamaño
una línea puede llegar fragmentada
*/
void get_buffer()
{
    char buffer[1024];

    while (std::fgets(buffer, sizeof(buffer), stdin) != nullptr) {
        fmt::print("{}", buffer);
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1) {
        fmt::println("validando argumentos:");

        for (int i = 1; i < argc; ++i) {
            fmt::println("  {}", argv[i]);
        }

        // return 0;
    }

    if (stdin_is_redirected()) {
        fmt::println("Ejecutando en modo pipe/redirección");
        std::string line;

        get_line();
        // get_buffer();

        return 0;
    }

    // fmt::println(stderr, "No se recibieron argumentos ni datos por stdin");
    return 0;
}
