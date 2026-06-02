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
    auto result = dividir(10, 2);

    if (result) {
        fmt::println("Resultado: {}", result.value());
    } else {
        fmt::println("Error: {}", result.error());
    }

    result = dividir(10, 0);
    if (!result) {
        fmt::println("Error: {}", result.error());
    } else {
        fmt::println("Resultado: {}", *result); // result.value()
    }

    return 0;
}
