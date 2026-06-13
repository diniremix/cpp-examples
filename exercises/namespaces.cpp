#include "namespaces.hpp"

/*
 * Un namespace no "encapsula" como una clase,
 * solo agrupa nombres. No existe "private",
 * lo más cercano es utilizar un namespace anónimo
 * `namespace { ... }` ó 'detail' que por convención
 * se usa `namespace XXXX::detail { ... }`
 */

// Namespace nombrado - todo público
namespace app {
    // namespace anónimo ("privado") real del
    // archivo (no exportado)
    namespace {
        bool is_valid_name(std::string name)
        {
            return name.size() >= 3;
        }

        bool is_valid_age(int age)
        {
            return age >= 0 && age <= 120;
        }
    } // namespace

    void create_user(std::string name, int age)
    {
        if (!is_valid_name(name)) {
            fmt::println(stderr, "invalid name");
            return;
        }

        if (!is_valid_age(age)) {
            fmt::println(stderr, "invalid age");
            return;
        }

        User user{name, age};
        print_user(user);
    }

    void print_user(const User& user)
    {
        fmt::println("User: {} ({})", user.name, user.age);
    }

} // namespace app

// esto NO es privado de verdad, solo es
//  una convención semántica.
namespace app::detail {

    bool is_strong_password(std::string password)
    {
        return password.size() >= 8;
    }

} // namespace app::detail

int main()
{
    app::create_user("Charles", 25);

    app::User u{"Jane", 30};
    app::print_user(u);
    bool is_strong = app::detail::is_strong_password("The quick brown fox jumps over the lazy dog");
    fmt::println("is_strong_password: {}", is_strong);

    return 0;
}
