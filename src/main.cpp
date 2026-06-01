#include <expected>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

std::expected<int, std::string> dividir(int a, int b)
{
    if (b == 0)
        return std::unexpected("División por cero");
    return a / b;
}

int main()
{
    spdlog::info("Application started");
    auto resultado = dividir(10, 2);

    if (resultado) {
        // std::println("Resultado: {}", resultado.value()); // C++23
        // std::print/println
        fmt::println("Resultado: {}", *resultado); // resultado.value()
    } else {
        // std::println("Error: {}", resultado.error()); // C++23 std::print/println
        fmt::println("Error: {}", resultado.error());
    }

    return 0;
}
