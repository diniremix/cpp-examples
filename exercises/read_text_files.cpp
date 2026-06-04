#include <filesystem> // Opcional pero útil (C++17)
#include <fmt/core.h>
#include <fstream> // Para ifstream, ofstream, fstream
#include <string>
// #include <sstream>

namespace fs = std::filesystem;

int main()
{
    fmt::println("manejo de archivos(lectura)");

    fs::path file_path = "sample_text_file.txt";

    if (!fs::exists(file_path)) {
        fmt::println(stderr, "The file '{}' does not exist; exiting...", file_path.string());
        return 1;
    }

    std::ifstream archivo(file_path);

    if (!archivo.is_open()) {
        fmt::println(stderr, "Error: The file '{}' could not be opened", file_path.string());
        return 1;
    }

    fmt::println("leyendo archivo '{}'...", file_path.string());
    fmt::println("");

    // Leer el archivo completo usando std::ostringstream
    std::ostringstream all_content;
    all_content << archivo.rdbuf(); // Lee todo el contenido
    fmt::print("{}\n", all_content.str());

    fmt::println("========");

    // para reutilizar el std::ifstream
    // limpiar flags de error (EOF)
    archivo.clear();
    // volver al inicio
    archivo.seekg(0);

    //
    // Leer línea por línea
    std::string linea;

    while (std::getline(archivo, linea)) {
        fmt::println("{}", linea);
    }

    fmt::println("========");

    //
    archivo.clear();
    // volver al inicio
    archivo.seekg(0);

    // leer todo el contenido en un std::string
    std::string content(std::istreambuf_iterator<char>{archivo}, std::istreambuf_iterator<char>{});

    fmt::println("{}", content);

    // El destructor de std::ifstream cerrará el archivo automáticamente
    archivo.close(); // no es ya necesario

    fmt::println("");
    fmt::println("saliendo...");
    return 0;
}
