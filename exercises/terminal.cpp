#include <charconv>
#include <chrono>
#include <fmt/color.h>
#include <fmt/core.h>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

constexpr auto reset = "\033[0m";
constexpr auto red = "\033[31m";
constexpr auto green = "\033[32m";
constexpr auto yellow = "\033[33m";
constexpr auto blue = "\033[34m";
constexpr auto cyan = "\033[36m";

void move_cursor_to(int row, int col)
{
    fmt::print("\033[{};{}H", row, col);
}

void cls()
{
    fmt::print("\033[2J\033[H");
}

int to_int(const std::string& s)
{
    int value{};

    auto first = s.data();
    auto last = first + s.size();

    auto [ptr, ec] = std::from_chars(first, last, value);

    if (ec != std::errc{}) {
        throw std::runtime_error("invalid integer");
    }

    return value;
}

std::string read_line(const std::string& prompt)
{
    fmt::print("{}", prompt);

    std::string line;
    std::getline(std::cin, line);

    return line;
}

bool parse_bool(std::string_view sv)
{
    return sv == "y" || sv == "Y" || sv == "yes" || sv == "true";
}

int main()
{

    fmt::println("\033[31mError\033[0m");
    fmt::println("\033[32mOK\033[0m");
    fmt::println("\033[33mWarning\033[0m");

    fmt::print(fg(fmt::color::green), "Servidor iniciado\n");
    fmt::print(fg(fmt::color::red), "Error de conexión\n");

    fmt::println("{}Civilians work!{}", red, reset);
    fmt::println("{}Soldiers fight!{}", blue, reset);
    fmt::println("{}Servidor iniciado{}", green, reset);
    fmt::println("{}the rich have lots of money!{}", yellow, reset);
    fmt::println("{}The poor have no money...{}", cyan, reset);

    for (int i = 0; i <= 100; ++i) {
        fmt::print("\r[{:3}%]", i);
        std::fflush(stdout);

        std::this_thread::sleep_for(std::chrono::milliseconds(5)); // 25
    }

    fmt::println("");

    std::vector<std::string> options = {"Start", "Settings", "Exit"};

    for (size_t i = 0; i < options.size(); ++i) {
        fmt::println("[{}] {}", i, options[i]);
    }

    // fmt::print("Select: ");
    // int choice{};
    // std::cin >> choice;

    auto line = read_line("Select: ");
    int choice = to_int(line);

    if (choice < 0 || choice >= static_cast<int>(options.size())) {
        fmt::println(stderr, "Invalid option");
        return 1;
    }

    fmt::println("Selected: {}", options[choice]);

    line = read_line("Name: ");
    fmt::println("name: {}", line);

    line = read_line("Age: ");
    int age = to_int(line);
    fmt::println("Age: {}", age);

    line = read_line("ready?: [y|Y|yes|true] ");
    bool resp = parse_bool(line);
    fmt::println("response: {}", resp);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    move_cursor_to(1, 10);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    cls();

    return 0;
}
