#include <filesystem> // Opcional pero útil (C++17)
#include <fmt/core.h>
#include <fstream> // Para ifstream, ofstream, fstream

namespace fs = std::filesystem;

int main()
{
    fmt::println("manejo de archivos(escritura)");

    fs::path file_path = "sample_text_file.txt";

    if (fs::exists(file_path)) {
        fmt::println(stderr, "The file exists; overwriting...");
    }

    std::ofstream archivo{file_path};
    if (!archivo) {
        fmt::println(stderr, "file is not writable");
        return 1;
    }

    archivo << "Hola desde C++\n";
    archivo << "" << "\n";
    archivo << "The standard Lorem Ipsum passage, used since the 1500s" << "\n";
    archivo << "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et "
               "dolore magna aliqua."
            << "\n";
    archivo << "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
               "consequat."
            << "\n";

    // Escribir múltiples líneas
    archivo << R"(Línea 3: Texto raw
        con saltos
            y tabs)";

    archivo.close(); // opcional

    fmt::println("saliendo...");
    return 0;
}
