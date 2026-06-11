#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <execution>
#include <fmt/core.h>
#include <mutex>
#include <ranges>
#include <thread>
#include <vector>
/*
 * Un hilo es una unidad de ejecución real, gestionada
 * por el sistema operativo.
 *
 * - `std::thread`
 * hay que llamar a `join()` o `detach()`.
 * si destruyes el objeto sin hacerlo, el programa termina.
 * - `std::jthread`
 * Introducido en C++20.
 * hace `join()` automáticamente al destruirse.
 * la mejor opción a utilizar.
 *
 * - Compartir datos entre hilos
 * `std::mutex`
 * en secciones críticas.
 * `std::condition_variable`
 * permite que un hilo espere una señal.
 *
 * - Hilos vs Procesos
 * Hilos: comparten memoria.
 * Procesos: cada proceso tiene su propia memoria.
 *
 * - `std::execution::par`
 * indica que la implementación puede repartir el
 * trabajo entre múltiples núcleos(similar a un pool de workers)
 */

std::mutex mutex;
std::condition_variable cv;

bool ready = false;

void my_task(int num, std::string name)
{
    fmt::println("Tarea en ejecución: {}, desde '{}'", num, name);
}

void task_await()
{
    fmt::println("task_await: esperando");

    std::unique_lock lock(mutex);

    cv.wait(lock, [] { return ready; });

    fmt::println("task_await: regresando");
}

int main()
{
    fmt::println("coroutine en C++23");
    // Llamar a las coroutines directamente
    my_task(1, "alone"); // Ajustado: Ejecutando sin hilos

    for (const int& i : std::views::iota(1, 10)) {
        // no espera(detach)
        std::thread([counter = std::move(i)] { my_task(counter, "thread(detach)"); }).detach();
        // join
        std::thread([counter = std::move(i)] { my_task(counter, "thread(join)"); }).join();
        // en cuasi orden, join automatico
        std::jthread([counter = std::move(i)] { my_task(counter, "jthread"); });
    }

    // un ejemplo de un hilo esperando una señal
    // mediante una `condition_variable`.
    std::jthread worker(task_await);

    fmt::println("main: haciendo otras cosas");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        std::lock_guard lock(mutex);
        ready = true;
    }

    cv.notify_one();

    fmt::println("main: señal enviada");

    //
    fmt::println("terminado");
    // Permite que la tarea finalize, si no se utiliza jthread
    // std::this_thread::sleep_for(std::chrono::seconds(3));

    // utilizando std::execution::par
    std::vector<int> values(1'000'000);
    std::for_each(std::execution::par, values.begin(), values.end(), [](int& value) { value *= 2; });

    return 0;
}
