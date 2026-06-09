#include <chrono>
#include <coroutine>
#include <format>
#include <iostream>
#include <ranges>
#include <thread>

struct Task {
    struct promise_type {
        Task get_return_object()
        {
            return {};
        }

        auto initial_suspend()
        {
            return std::suspend_never{};
        }

        auto final_suspend() noexcept
        {
            return std::suspend_always{};
        }
        void return_void() {}
        void unhandled_exception() {}
    };
};

Task my_task(int num)
{
    std::cout << std::format("Tarea en ejecución: {}\n", num);
    // co_await std::suspend_always{}; // Suspende la coroutine
    co_return;
}

int main()
{
    std::printf("coroutine en C++23\n");
    // my_task(); // Llama a la coroutine
    //
    /*
    //  Si deseas mantener la vida útil de la aplicación,
    //  puedes agregar lógica adicional aquí, como esperar un tiempo.
    for (const int& i : std::views::iota(1, 10)) {
        // joining thread.
        std::jthread([i] {
            task(i);
        });
        //

        std::thread t([i] {
            my_task(i);
        });
        //
        t.join();
        //

        // es equivalente a
        std::thread([i] {
            my_task(i); // Lanza la tarea en un hilo
        }).join();// espera a que termine.
    }
    */
    //
    for (const int& i : std::views::iota(1, 10)) {
        // Llama a la coroutine directamente
        // my_task(i); // Ajustado: Ejecutando sin hilos
        std::thread([counter = std::move(i)] {
            my_task(counter); // Lanza la tarea en un hilo
        }).detach();          // no espera

        // en cuasi orden
        std::jthread([counter = std::move(i)] {
            my_task(counter); // Lanza la tarea en un hilo
        });                   // automatico
        //
        // std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Pausa para ver la salida
    }

    // Permite que la tarea se ejecute
    std::this_thread::sleep_for(std::chrono::seconds(6));

    return 0;
}
