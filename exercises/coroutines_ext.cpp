#include <asio.hpp>
#include <asio/steady_timer.hpp>
#include <chrono>
#include <fmt/core.h>

#ifdef __cpp_lib_generator
    #include <generator>
#else
    // fallback
    #include "generator.hpp"
#endif

asio::awaitable<void> toggle_switch(std::string name, int times, asio::steady_timer& timer)
{
    bool state = false;

    for (int i = 0; i < times; ++i) {

        state = !state;

        fmt::println("{}: {}", name, state ? "ON" : "OFF");

        timer.expires_after(std::chrono::seconds(1));

        co_await timer.async_wait(asio::use_awaitable);
    }
}

asio::awaitable<void> toggle_switch2(std::string name, int times)
{
    auto executor = co_await asio::this_coro::executor;
    asio::steady_timer timer(executor);

    bool state = false;

    for (int i = 0; i < times; ++i) {

        state = !state;

        fmt::println("{}: {}", name, state ? "ON" : "OFF");

        timer.expires_after(std::chrono::seconds(1));

        co_await timer.async_wait(asio::use_awaitable);
    }
}

asio::awaitable<int> sum_with_progress(int limit)
{
    int total = 0;

    for (int i = 1; i <= limit; ++i) {
        total += i;

        fmt::println("procesando {}", i);
    }

    co_return total;
}

asio::awaitable<void> run()
{
    auto executor = co_await asio::this_coro::executor;
    asio::steady_timer timer(executor);

    co_await toggle_switch("A1", 5, timer);
    co_await toggle_switch("B1", 5, timer);
    fmt::println("Finalizado");
}

//
// usando generadores (`std::generator`)
//

// Nuestra corutina generadora
std::generator<int> even_numbers(int start)
{
    int n = start;
    while (true) {
        if (n % 2 != 0)
            n++; // Asegurar que es par
        co_yield n;
        n += 2;
    }
}

// Nuestra corutina generadora de valores boleanos
std::generator<bool> toggle_switch_gen(int times)
{
    bool state = false;
    for (int i = 0; i < times; ++i) {
        state = !state; // Invierte el valor
        co_yield state;
    }
}

// Nuestra corutina generadora de string_view
std::generator<std::string_view> week_days()
{
    co_yield "Lunes";
    co_yield "Martes";
    co_yield "Miércoles";
    co_yield "Jueves";
    co_yield "Viernes";
    co_yield "Sábado";
    co_yield "Domingo";
}

// sumatoria usando Yield de estados
struct Progress {
    int current;
    int partial_sum;
};

// utilizando un struct como acumulador
std::generator<Progress> sum_with_progress_gen(int limit)
{
    int total = 0;

    for (int i = 1; i <= limit; ++i) {
        total += i;
        co_yield {i, total};
    }
}

// utilizando `std::views::iota`
auto partial_sums(int limit)
{
    return std::views::iota(1, limit + 1) | std::views::transform([sum = 0](int x) mutable {
               sum += x;
               return sum;
           });
}

int main()
{
    fmt::println("generadores...");

    fmt::println("even_numbers");
    fmt::println("Primeros 5 números pares empezando desde 10:");
    for (auto const& v : even_numbers(10) | std::views::take(10)) {
        fmt::print("{} ", v);
    }

    fmt::println("");
    fmt::println("boleanos aleatorios:");
    for (auto const& v : toggle_switch_gen(5)) {
        fmt::print("{} ", v);
    }

    fmt::println("");
    fmt::println("Días de la semana:");
    for (auto const& v : week_days()) {
        fmt::print("{} ", v);
    }

    fmt::println("");
    fmt::println("sumatoria:");

    int final_sum = 0;
    for (auto p : sum_with_progress_gen(5)) {
        fmt::println("i={}, sum={}", p.current, p.partial_sum);
        final_sum = p.partial_sum;
    }
    fmt::println("final_sum: {}", final_sum);

    fmt::println("");
    fmt::println("partial_sums:");
    for (auto s : partial_sums(5)) {
        fmt::println("{}", s);
    }

    fmt::println("============================");
    fmt::println("coroutines con Asio...");
    asio::io_context io_context;

    fmt::println("run(toggle_switch)");
    //  asio::detached omite el resultado
    asio::co_spawn(io_context, run(), asio::detached);

    fmt::println("toggle_switch2");
    asio::co_spawn(io_context, toggle_switch2("A2", 5), asio::detached);
    asio::co_spawn(io_context, toggle_switch2("B2", 5), asio::detached);

    fmt::println("sum_with_progress");
    // capturando el resultado
    asio::co_spawn(io_context, sum_with_progress(5), [](std::exception_ptr ex, int result) {
        if (ex) {
            try {
                std::rethrow_exception(ex);
            } catch (const std::exception& e) {
                fmt::println(stderr, "{}", e.what());
            }
            return;
        }

        fmt::println("Resultado: {}", result);
    });

    // crear el time con io_context
    asio::steady_timer timer(io_context);
    asio::co_spawn(io_context, toggle_switch("A3", 5, timer), asio::detached);

    // crear el time con std::make_shared
    auto timer_md = std::make_shared<asio::steady_timer>(io_context);
    asio::co_spawn(io_context, toggle_switch("B3", 5, *timer_md), asio::detached);

    // Ejecutamos el bucle de eventos de Asio
    io_context.run();

    return 0;
}
