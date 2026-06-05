#include <expected>
#include <filesystem>
#include <fmt/core.h>
#include <format>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Parser CSV simple, usando Structs para mapear campos
// No soporta comillas ni comas escapadas.

struct Registro {
    std::string nombre;
    int edad;
    double salario;

    // OK
    // Validación básica de cada campo
    // static std::optional<Registro> parse_csv_line(std::string_view line)
    static std::optional<Registro> parse_csv_line(const std::string& line)
    {
        std::istringstream ss(line);
        std::string token;
        Registro reg;
        std::string extra; // extra

        // nombre
        if (!std::getline(ss, reg.nombre, ','))
            return std::nullopt;

        // token para el sgte campo (edad)
        if (!std::getline(ss, token, ','))
            return std::nullopt;

        // edad
        try {
            reg.edad = std::stoi(token);
        } catch (const std::exception& e) {
            fmt::println(stderr, "from_csv_line edad Error: {}", e.what());
            return std::nullopt;
        }

        // token para el sgte campo (salario)
        if (!std::getline(ss, token, ','))
            return std::nullopt;

        // salario
        try {
            reg.salario = std::stod(token);
        } catch (const std::exception& e) {
            fmt::println(stderr, "from_csv_line salario Error: {}", e.what());
            return std::nullopt;
        }

        // si hay campos extras, salir
        if (ss.rdbuf()->in_avail() > 0) {
            return std::nullopt;
        }

        return reg;
    }
};

// OK
// Función para leer archivo CSV
std::vector<Registro> read_csv(const fs::path& ruta)
{
    std::vector<Registro> registros;
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        fmt::println(stderr, "Error: No se pudo abrir '{}'", ruta.string());
        return registros;
    }

    std::string line;
    // si queremos saltar la cabecera
    // std::getline(archivo, linea);

    // leer la cabecera
    if (!std::getline(archivo, line))
        return registros;

    // validar la cabecera
    if (line != "nombre,edad,salario") {
        fmt::println(stderr, "csv headers inesperado");
        // salir
    }

    std::size_t num_linea = 1;
    while (std::getline(archivo, line)) {
        ++num_linea;
        if (line.empty())
            continue;

        auto registro = Registro::parse_csv_line(line);
        if (registro) {
            registros.push_back(registro.value()); // *registro
        } else {
            fmt::println(stderr, "Advertencia: Línea No. {} inválida: '{}'", num_linea, line);
        }
    }

    return registros;
}

// TODO
// Función para escribir resultados a
// un nuevo archivo csv
static std::expected<void, std::string> write_csv(const fs::path& ruta, const std::vector<Registro>& registros)
{
    std::ofstream archivo(ruta);

    if (!archivo.is_open()) {
        std::string msg = std::format("Error: No se pudo crear '{}'", ruta.string());
        fmt::println(stderr, "{}", msg);
        return std::unexpected(msg);
    }

    // crear cabecera del archivo
    archivo << "nombre,edad,salario\n";

    // escribir datos del resultado
    for (const auto& reg : registros) {
        archivo << reg.nombre << "," << reg.edad << "," << reg.salario << "\n";
    }

    fmt::println("");
    fmt::println("Resultados guardados en '{}'", ruta.string());
    // aqui se cierra explicitamente para
    // obtener el tamaño en bytes
    archivo.close();

    fmt::println("  Tamaño: {} bytes", fs::file_size(ruta));

    return {};
}

// OK
static std::expected<std::vector<Registro>, std::string> read_employee_csv(const fs::path& employee_file)
{
    fmt::println("Leyendo '{}'...", employee_file.string());
    auto registros = read_csv(employee_file);

    if (registros.empty()) {
        fmt::println(stderr, "No se encontraron registros válidos");
        return std::unexpected("Missing valid record");
    }

    // Procesar datos (ej: filtrar por edad > 28)
    fmt::println("");
    fmt::println("Registros originales: {}", registros.size());

    std::vector<Registro> filtrados;
    double total_salarios = 0;

    for (const auto& reg : registros) {
        if (reg.edad > 28) {
            filtrados.push_back(reg);
            total_salarios += reg.salario;
        }
    }

    fmt::println("Registros filtrados (edad > 28): {}", filtrados.size());
    if (!filtrados.empty()) {
        fmt::println("Salario promedio: ${:.2f}", total_salarios / filtrados.size());
    }

    fmt::println("Información del archivo: '{}'", employee_file.string());
    fmt::println("  Tamaño: {} bytes", fs::file_size(employee_file));

    return filtrados;
}

// OK
void create_employee_csv(const fs::path& employee_file)
{
    if (!fs::exists(employee_file)) {
        fmt::println("Creando archivo de ejemplo '{}'...", employee_file.string());
        std::ofstream the_file(employee_file);

        the_file << "nombre,edad,salario\n";
        the_file << "Katie,30,50000.50\n";
        the_file << "Luke,25,45000.75\n";
        the_file << "John,invalido,40000\n"; // Línea inválida para pruebas
        the_file << "Jane,29,27000\n";
        the_file << "Charlie,35,60000.00\n";

        fmt::println("Información del archivo:");
        // aqui se cierra explicitamente para
        // obtener el tamaño en bytes
        the_file.close();

        fmt::println("  Tamaño: {} bytes", fs::file_size(employee_file));
    }
}

int main()
{
    // Crear archivo CSV de ejemplo (si no existe)
    fmt::println("Manejo de archivos CSV");
    fmt::println("");

    fs::path archivo_entrada = "empleados.csv";
    fs::path archivo_salida = "resultados.csv";

    create_employee_csv(archivo_entrada);

    auto filters_record = read_employee_csv(archivo_entrada);
    if (!filters_record) {
        fmt::println("Error: {}", filters_record.error());
    }

    auto result = write_csv(archivo_salida, filters_record.value());
    if (!result) {
        fmt::println("Error: {}", result.error());
    }

    return 0;
}
