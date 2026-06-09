#include <asio.hpp>
#include <asio/steady_timer.hpp>
#include <fmt/core.h>
// #include <generator>

#include <chrono>

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

// main
int main()
{
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
