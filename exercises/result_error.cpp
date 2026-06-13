#include <expected>
#include <fmt/core.h>
#include <fmt/format.h>
#include <numbers>
#include <string>

/*
 * std::expected<T, E>
 * - T representa el valor exitoso.
 * - E representa el tipo de error.
 *
 * std::unexpected
 * - Se utiliza para construir y devolver un error (E).
 *
 * and_then()
 * - Se ejecuta únicamente si el expected contiene un valor válido.
 * - La función o lambda recibe el valor exitoso (T).
 * - Debe devolver otro std::expected.
 * - Todas las operaciones encadenadas con and_then() deben
 *  usar exactamente el mismo tipo de error (E).
 * - Permite encadenar operaciones que también pueden fallar.
 *
 * transform()
 * - Se ejecuta únicamente si el expected contiene un valor válido.
 * - La función o lambda recibe el valor exitoso (T).
 * - Debe devolver un valor normal, no un std::expected.
 * - Transforma el tipo o contenido del valor exitoso.
 * - El tipo de error permanece sin cambios.
 *
 * transform_error()
 * - Se ejecuta únicamente si el expected contiene un error.
 * - La función o lambda recibe el error (E).
 * - Debe devolver un nuevo error.
 * - Transforma el tipo o contenido del error.
 * - El valor exitoso permanece sin cambios.
 *
 * Resumen:
 * - and_then()      -> expected<T,E> -> expected<U,E>
 * - transform()     -> expected<T,E> -> expected<U,E>
 * - transform_error()-> expected<T,E> -> expected<T,F>
 *
 * and_then() -> cambia el valor y sigue pudiendo fallar
 * transform() -> cambia el valor y no puede fallar
 * transform_error() ->cambia únicamente el error
 */

enum class Error {
    DivisionByZero,
    NegativeValue,
};

// convertimos cada enum de `Error` a un string legible
// std::string_view error_to_string(Error error)
std::string error_to_string(Error error)
{
    switch (error) {
    case Error::DivisionByZero:
        return "division by zero";
    case Error::NegativeValue:
        return "negative value";
    default:
        return "unknown error";
    }
}

// podemos utilizar un formatter de `fmt/std`
template <> struct fmt::formatter<Error> {
    constexpr auto parse(fmt::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    // si no se crea la función `error_to_string`
    /*
    auto format(Error error, std::format_context& ctx) const
    {
        std::string_view msg;
        switch (error) {
        case Error::DivisionByZero:
            msg = "division by zero";
            break;
        case Error::NegativeValue:
            msg = "negative value";
            break;
        default:
            msg = "unknown error";
            break;
        }
        return std::format_to(ctx.out(), "{}", msg);
    }
    */

    // reutilizar `error_to_string` en este caso
    auto format(Error error, auto& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", error_to_string(error));
    }
};

// alias de tipos usando `using`
using DoubleResult = std::expected<double, Error>;
using StringResult = std::expected<std::string, Error>;

// equivalente a `std::expected<double, Error> divide(int a, int b)`
DoubleResult divide(int a, int b)
{
    if (b == 0) {
        return std::unexpected(Error::DivisionByZero);
    }

    return static_cast<double>(a) / b;
}

// equivalente a `std::expected<std::string, Error> format_double(double value)`
StringResult format_double(double value)
{
    if (value < 0) {
        return std::unexpected(Error::NegativeValue);
    }

    return fmt::format("{:.2f}", value);
}

int main()
{
    // 1st
    auto result = divide(100, 3)
                      .transform([](double value) { return value * std::numbers::pi; })
                      .and_then(format_double)
                      .transform([](const std::string& text) { return "Valor final: " + text; })
                      // utilizando `switch (error)`y validacion normal
                      /*
                        .transform_error([](Error error) {
                          switch (error) {
                          case Error::DivisionByZero:
                              return std::string{"division by zero"};
                          case Error::NegativeValue:
                              return std::string{"negative value"};
                          }
                          return std::string{"unknown error"};
                      });
                      */
                      // utilizando fmt
                      //.transform_error([](Error error) { return fmt::format("{}", error); });
                      // utilizando error_to_string
                      .transform_error([](Error error) { return error_to_string(error); });

    if (result) {
        fmt::println("Resultado(1): {}", *result);
    } else {
        fmt::println(stderr, "Error(1): {}", result.error());
    }

    // 2nd
    // auto result2 = divide(100, 0)
    result = divide(100, 0)
                 .transform([](double value) { return value * 1.19; })
                 .and_then(format_double)
                 .transform([](const std::string& text) { return "Valor final: " + text; })
                 .transform_error([](Error error) { return fmt::format("{}", error); });

    if (result) {
        fmt::println("Resultado(2): {}", *result);
    } else {
        fmt::println(stderr, "Error(2): {}", result.error());
    }
}
