#pragma once
#include <fmt/core.h>
#include <string>

namespace app {

    struct User {
        std::string name;
        int age;
    };

    void create_user(std::string name, int age);
    void print_user(const User& user);

} // namespace app

namespace app::detail {

    bool is_strong_password(std::string password);

}
