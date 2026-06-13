#include <format>
#include <iostream>

// pruebas realizadas
// | Plataforma              | fmt | std::format | std::print |
// | ----------------------- | --- | ----------- | ---------- |
// | Ubuntu 24.04 (GCC 13)   | X   | X (202110)  |            |
// | Fedora 44 (GCC 16)      | X   | X (202304)  | X (202406) |
// | Windows 11 (MSVC 19.44) | X   | X (202304)  | X (202406) |

#ifdef __cpp_lib_print
    #include <print>
#endif

#ifdef HAS_FMT
    #include "structs.hpp"

    #include <fmt/core.h>
    #include <fmt/format.h>
    #include <fmt/ostream.h>
#endif

// ============================================================================
// revisar el soporte de las librerías
// ============================================================================

void print_capabilities()
{
    std::cout << "=== Formatting Support ===\n";

#ifdef HAS_FMT
    std::cout << "FMT_VERSION       = " << FMT_VERSION << '\n';
#else
    std::cout << "FMT_VERSION(HAS_FMT)       = unavailable\n";
#endif

#ifdef __cpp_lib_format
    std::cout << "__cpp_lib_format  = " << __cpp_lib_format << '\n';
#else
    std::cout << "__cpp_lib_format  = unavailable\n";
#endif

#ifdef __cpp_lib_print
    std::cout << "__cpp_lib_print   = " << __cpp_lib_print << '\n';
#else
    std::cout << "__cpp_lib_print   = unavailable\n";
#endif

    std::cout << '\n';
}

// ============================================================================
// Example 1: ostream
// ============================================================================

struct PuntoOstream {
    int x;
    int y;

    friend std::ostream& operator<<(std::ostream& os, const PuntoOstream& p)
    {
        return os << "Punto(" << p.x << ", " << p.y << ")";
    }
};

void example_ostream()
{
    std::cout << "=== std::ostream ===\n";

    PuntoOstream p{10, 20};

    std::cout << p << '\n';
    std::cout << '\n';
}

// ============================================================================
// Example 2: fmt::streamed
// ============================================================================

#ifdef HAS_FMT

void example_fmt_streamed()
{
    fmt::println("=== fmt::streamed ===");

    PuntoOstream p{10, 20};

    fmt::println("{}", fmt::streamed(p));
    fmt::println("");
}

#endif

// ============================================================================
// Example 3: fmt::ostream_formatter
// ============================================================================

#ifdef HAS_FMT

struct PuntoFmtOstream {
    int x;
    int y;

    friend std::ostream& operator<<(std::ostream& os, const PuntoFmtOstream& p)
    {
        return os << "Punto(" << p.x << ", " << p.y << ")";
    }
};

template <> struct fmt::formatter<PuntoFmtOstream> : fmt::ostream_formatter {
};

void example_fmt_ostream_formatter()
{
    fmt::println("=== fmt::ostream_formatter ===");

    PuntoFmtOstream p{10, 20};

    fmt::println("{}", p);
    fmt::println("");
}

#endif

// ============================================================================
// Example 4: fmt::formatter
// ============================================================================

#ifdef HAS_FMT

struct PuntoFmtNative {
    int x;
    int y;
};

template <> struct fmt::formatter<PuntoFmtNative> {
    constexpr auto parse(fmt::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const PuntoFmtNative& p, fmt::format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "Punto({}, {})", p.x, p.y);
    }
};

void example_fmt_formatter()
{
    fmt::println("=== fmt::formatter ===");

    PuntoFmtNative p{10, 20};

    fmt::println("{}", p);
    fmt::println("");
}

#endif

// ============================================================================
// Example 5: std::formatter + std::print
// ============================================================================

#ifdef __cpp_lib_print

struct PuntoStdPrint {
    int x;
    int y;
};

template <> struct std::formatter<PuntoStdPrint> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const PuntoStdPrint& p, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "Punto({}, {})", p.x, p.y);
    }
};

void example_std_print()
{
    std::println("=== std::print + std::formatter ===");

    PuntoStdPrint p{10, 20};

    std::println("{}", p);
    std::println();
}

#endif

int main()
{
    print_capabilities();

    example_ostream();

#ifdef HAS_FMT
    example_fmt_streamed();
    example_fmt_ostream_formatter();
    example_fmt_formatter();
    //
    Books new_book = {1, "1984", "George Orwell"};
    fmt::println("utilizando fmt::formatter (implementado en 'structs.hpp')");
    fmt::println("new_book: {}", new_book);
    fmt::println("");
#endif

#ifdef __cpp_lib_print
    example_std_print();
#endif

    return 0;
}
