#include <coroutine>
#include <exception>
#include <fmt/core.h>
// #include <iostream>
#include <string>

// Definimos nuestro tipo Generator
template <typename T> struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    // La estructura interna que gestiona el estado
    struct promise_type {
        T current_value;
        std::exception_ptr exception_ptr;

        // Se llama al inicio de la corutina
        Generator get_return_object()
        {
            return Generator(handle_type::from_promise(*this));
        }

        // Suspender al inicio para que no empiece hasta que la pidamos
        std::suspend_always initial_suspend()
        {
            return {};
        }

        // Suspender al final para permitir iterar hasta el fin
        std::suspend_always final_suspend() noexcept
        {
            return {};
        }

        // Llamado por co_yield
        template <std::convertible_to<T> From> std::suspend_always yield_value(From&& from)
        {
            current_value = std::forward<From>(from);
            return {};
        }

        // termina sin devolver valores
        void return_void() {}

        // Si dentro de la función ocurre un error
        // el compilador captura la excepción y
        // llama a este método.
        void unhandled_exception()
        {
            exception_ptr = std::current_exception();
        }
    };

    handle_type handle;

    explicit Generator(handle_type h) : handle(h) {}

    ~Generator()
    {
        if (handle)
            handle.destroy();
    }

    // Operador para obtener el siguiente valor
    T next()
    {
        handle.resume();
        if (handle.promise().exception_ptr) {
            std::rethrow_exception(handle.promise().exception_ptr);
        }
        return handle.promise().current_value;
    }

    // Comprobar si la corutina ha terminado
    bool is_done() const
    {
        return handle.done();
    }
};

// Nuestra corutina generadora
Generator<int> even_numbers(int start)
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
Generator<bool> toggle_switch(int times)
{
    bool state = false;
    for (int i = 0; i < times; ++i) {
        state = !state; // Invierte el valor
        co_yield state;
    }
}

// con strings funciona porque std::string es
// convertible(`std::convertible_to<T>`) desde
// literales de cadena (`const char*`).
Generator<std::string> week_days()
{
    co_yield "Lunes";
    co_yield "Martes";
    co_yield "Miércoles";
    co_yield "Jueves";
    co_yield "Viernes";
    co_yield "Sábado";
    co_yield "Domingo";
}

int main()
{
    try {
        auto gen = even_numbers(10);

        fmt::println("Primeros 5 números pares empezando desde 10:");
        for (int i = 0; i < 5; ++i) {
            if (!gen.is_done()) {
                fmt::print("{} ", gen.next());
            }
        }

        fmt::println("");

        fmt::println("boleanos aleatorios:");
        // Para los booleanos
        auto bool_gen = toggle_switch(5);
        fmt::print("Estados: ");
        while (!bool_gen.is_done()) {
            fmt::print("{} ", bool_gen.next() ? "ON" : "OFF");
        }

        fmt::println("");

        // Para los strings
        auto day_gen = week_days();
        fmt::println("Días de la semana:");
        while (!day_gen.is_done()) {
            fmt::println("- {}", day_gen.next());
        }

    } catch (const std::exception& e) {
        fmt::println(stderr, "Error: {}", e.what());
    }

    return 0;
}
