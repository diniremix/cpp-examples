#include <fmt/core.h>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

struct Usuario {
    std::string nombre;
    int edad;
};

int main()
{
    std::vector<Usuario> usuarios = {{"Anne", 25}, {"Charles", 30}, {"Mary", 28}, {"Jane", 35}};

    // 1. Range-based for (`for usuario in usuarios` en Python)
    fmt::println("");
    fmt::println("=== Todos los usuarios ===");
    for (const auto& usuario : usuarios) {
        fmt::println("{} tiene {} años", usuario.nombre, usuario.edad);
    }

    // 2. For tradicional con índice
    fmt::println("");
    fmt::println("=== Con índice ===");

    for (size_t i = 0; i < usuarios.size(); ++i) {
        fmt::println("[{}] {}", i, usuarios[i].nombre);
    }

    // 3. While
    fmt::println("");
    fmt::println("=== Buscando usuario ===");

    std::string buscar = "Ken";
    size_t index = 0;
    bool encontrado = false;

    while (index < usuarios.size()) {
        if (usuarios[index].nombre == buscar) {
            fmt::println("Encontrado: {} en posición {}", buscar, index);
            encontrado = true;
            break; // Similar a break en Python
        }
        ++index;
    }

    if (!encontrado) {
        fmt::println("Usuario: '{}' no encontrado", buscar);
    }

    // 4. Do-while
    fmt::println("");
    fmt::println("=== Validación de entrada ===");

    int opcion;

    do {
        fmt::println("Seleccione opción (1-3): ");
        std::cin >> opcion;
    } while (opcion < 1 || opcion > 3);

    fmt::println("Opción seleccionada: {}", opcion);

    fmt::println("");
    fmt::println("=== Tablas de multiplicar=== ");

    for (size_t tabla = 1; tabla < 11; ++tabla) {
        for (size_t num = 1; num < 11; ++num) {
            fmt::println("{} * {} = {}", tabla, num, tabla * num);
        }
        fmt::println("");
    }

    // 5. Range, range es inclusivo
    fmt::println("");
    fmt::println("=== uso de <ranges>=== ");
    for (const int& i : std::views::iota(1, 10)) {
        fmt::println("{}", i);
    }
    return 0;
}
