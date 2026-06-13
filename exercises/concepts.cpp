#include <concepts>
#include <fmt/core.h>
#include <ranges>
#include <string>
#include <vector>

/*
 * Los concepts permiten restringir templates, ofreciendo
 * restricciones de tipos, de forma explícita.
 * un 'concept' es básicamente una regla y al
 * aplicarla aclara que 'X función' solo acepta
 * tipos que cumplen la regla.
 * esto ayuda a expresar requisitos de forma entendible
 * documentar la intención del concepto y obtener
 * errores de compilación más claros
 */

struct User {
    std::string username;
    int age;
    bool status;
};

// se define un concepto de nombre `Formattable` y verificamos
// también el tipo retornado.
template <typename T>
concept Formattable = requires(T value) {
    { fmt::format("{}", value) } -> std::convertible_to<std::string>;
};

// se implementa el concepto a una función
// forma normal
template <Formattable T> void print_t(const T& value)
{
    fmt::println("{}", value);
}

// forma moderna
void print_f(const Formattable auto& value)
{
    fmt::println("{}", value);
}

// se implementa un formatter para `User`
template <> struct fmt::formatter<User> : fmt::formatter<std::string> {
    auto format(const User& user, fmt::format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "User('{}', {}, {})", user.username, user.age, user.status);
    }
};

/*
 * para este ejemplo queremos una función
 * acepte cualquier rango de usuarios.
 * No solo `std::vector<User>`
 * También:
 * `std::list<User>`
 * `std::array<User, N>`
 * vistas (views)
 */

// concept `UserRange`
template <typename R>
concept UserRange = std::ranges::input_range<R> && std::same_as<std::ranges::range_value_t<R>, User>;

// `active_usernames` solo acepta tipos que cumplen la regla `UserRange`
std::vector<std::string> active_usernames(const UserRange auto& users)
{
    // filtra por usuarios activos y devuelve un vector con los nombres
    auto names = users | std::views::filter([](const User& user) { return user.status; }) |
                 std::views::transform(
                     [](const User& user) { return fmt::format("user: '{}', status: Active", user.username); });

    return {names.begin(), names.end()};
}

// Reglas de negocio para nuestro concept
// pequeñas, puras y reutilizables.

constexpr bool is_adult(int age)
{
    return age >= 18;
}

constexpr bool is_active_adult(const User& user)
{
    return user.status && is_adult(user.age);
}

int main()
{
    fmt::println("hello world");
    print_t("hello concept");
    print_t(1.3);
    print_t(42);
    print_t(short{14});
    print_t(long{16});

    print_f("hello concept");
    print_f(1.3);
    print_f(42);
    print_f(short{14});
    print_f(long{16});

    print_f(""); // usando nuestra funcion

    User user{"Connor, John", 12, true};
    // ahora print_t|print_f pueden "imprimir" a `User`
    print_t(user);
    print_t(User{"Connor, Sarah", 33, false});

    print_t("");                          // usando nuestra funcion
    print_t("lista de usuarios activos"); // usando nuestra funcion

    // 1. creamos un vector de usuarios
    std::vector<User> users{{"alice", 25, true}, {"bob", 30, false}, {"charlie", 17, true}, {"david", 40, true}};

    auto names = active_usernames(users);

    for (const auto& name : names) {
        fmt::println("{}", name);
    }

    print_f("");                          // usando nuestra funcion
    print_f("usuarios activos, adultos"); // usando nuestra funcion

    // 2. filtra por usuarios activos, adultos
    auto user_list = users | std::views::filter([](const User& user) { return user.status; }) |
                     std::views::filter([](const User& user) { return is_adult(user.age); }) |
                     std::views::transform([](const User& user) { return user.username; });

    for (const auto& name : user_list) {
        fmt::println("{}", name);
    }

    print_f("");                              // usando nuestra funcion
    print_f("usuarios activos, adultos (2)"); // usando nuestra funcion

    // 3. igual que `2` solo que reutilizando las funciones de negocio,
    // mejorando la intención del resultado a obtener y haciendo el pipeline
    // mucho más legible, ahora la 'vista' ya no conoce la lógica de negocio.
    auto active_adult = users | std::views::filter(is_active_adult) | std::views::transform(&User::username);
    /*
     donde `std::views::transform(&User::username);` equivale a
     ```
        [](const User& user) {
            return user.username;
        }
    ```
    */

    for (const auto& name : active_adult) {
        fmt::println("{}", name);
    }

    print_t(""); // usando nuestra funcion
    return 0;
}
