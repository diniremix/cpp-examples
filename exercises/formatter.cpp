#include "structs.hpp"

#include <format>
#include <iostream>

struct Punto {
    int x, y;

    friend std::ostream& operator<<(std::ostream& os, const Punto& p)
    {
        return os << "Punto(" << p.x << ", " << p.y << ")";
    }
};

// utilizar std::print
#ifdef __cpp_lib_print
#include <print>
template <> struct std::formatter<Punto> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Punto& p, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "Punto({}, {})", p.x, p.y);
    }
};
#endif

#ifdef FMT_VERSION
#include <fmt/core.h>
#include <fmt/ostream.h>
// usa ostream_formatter
template <> struct fmt::formatter<Punto> : fmt::ostream_formatter {
};

// formatter
template <> struct std::formatter<Punto> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin(); // Ignoramos especificadores
    }

    auto format(const Punto& p, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "Punto({}, {})", p.x, p.y);
    }
};
#endif

int main()
{
#ifdef FMT_VERSION
    Books new_book = {1, "1984", "George Orwell"};
    fmt::println("utilizando fmt::formatter (implementado en 'structs.hpp')");
    fmt::println("new_book: {}", new_book);
    fmt::println("");

    Punto new_point{10, 20};

    fmt::println("utilizando std::ostream");
    std::cout << "new_point: " << new_point << std::endl;

    fmt::println("");
    fmt::println("utilizando fmt::ostream_formatter");
    fmt::print("new_point: {}\n", new_point);

    fmt::println("");
    fmt::println("Usa fmt::streamed como wrapper");
    fmt::print("new_point: {}\n", fmt::streamed(new_point));
#endif

#ifdef __cpp_lib_print
    Punto new_point{10, 20};
    std::println("");
    std::println("Usa print");
    std::print("new_point: {}\n", new_point);
#endif

    return 0;
}
