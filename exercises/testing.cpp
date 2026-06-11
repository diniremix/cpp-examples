#include <algorithm>
#include <cassert>
#include <fmt/core.h>
#include <stdexcept>
#include <type_traits>
#include <vector>

/*
 * - `assert` es una macro que verifica una condición en
 * tiempo de ejecución. Si la condición es falsa,
 * el programa termina inmediatamente mostrando un mensaje
 * de error con el archivo, línea y condición que falló.
 * - `static_assert`
 * condiciones en tiempo deompilación, condiciones estáticas.
 *
 * para otros casos mas avanzados, se recomienda utilizar
 * librerías especificas
 * - Catch2
 * - doctest
 * - Google Test
 * - Boost.Test
 */

template <typename T> T dividir(T a, T b)
{
    static_assert(std::is_arithmetic_v<T>, "T debe ser tipo numérico");
    static_assert(sizeof(T) <= 8, "tipo demasiado grande");
    return a / b;
}

void procesar(int* ptr, int valor)
{
    assert(ptr != nullptr && "puntero nulo: no se puede desreferenciar");
    assert(valor > 0 && "valor debe ser positivo");
    *ptr = valor;
}

float divide(int a, int b)
{
    assert(b != 0 && "divisor no puede ser cero");
    return static_cast<float>(a) / static_cast<float>(b);
}

void test_divide()
{
    assert(divide(10, 2) == 5.0f);
    assert(divide(3, 2) == 1.5f);
    assert(divide(-6, 3) == -2.0f);
    fmt::println("test_divide: OK");
}

int encontrar_maximo(const std::vector<int>& nums)
{
    assert(!nums.empty() && "vector vacío");
    return *std::max_element(nums.begin(), nums.end());
}

void test_maximo()
{
    std::vector<int> v1 = {1, 5, 3, 9, 2};
    assert(encontrar_maximo(v1) == 9);

    std::vector<int> v2 = {-5, -1, -8};
    assert(encontrar_maximo(v2) == -1);

    std::vector<int> v3 = {42};
    assert(encontrar_maximo(v3) == 42);

    fmt::println("test_maximo: OK");
}

float calcular_media(const std::vector<int>& datos)
{
    assert(!datos.empty() && "no se puede calcular media de vector vacío");

    int suma = 0;
    for (int valor : datos) {
        suma += valor;
    }
    return static_cast<float>(suma) / static_cast<float>(datos.size());
}

int main()
{
    test_divide();
    fmt::println("");
    test_maximo();
    fmt::println("");

    // demostración de error
    float resultado = divide(10, 3);
    fmt::println("10 / 3 = {:.2f}", resultado);

    std::vector<int> numeros = {10, 20, 30, 40};
    float media = calcular_media(numeros);
    fmt::println("media: {:.2f}", media);

    fmt::println("");
    // assert falla
    // std::vector<int> vacio;
    // calcular_media(vacio);

    int numero = 10;

    // caso 1: correcto
    procesar(&numero, 42);
    fmt::println("numero: {}", numero);
    fmt::println("");

    // assert falla
    // caso 2: valor negativo
    // procesar(&numero, -5);

    static_assert(sizeof(int) == 4, "int debe ser 4 bytes");
    static_assert(1 + 1 == 2, "matemáticas básicas fallaron");

    dividir(10, 2);
    fmt::println("");

    dividir<float>(1.1, 2.3);
    // error en compilación
    // dividir<std::string>("a", "b");

    fmt::println("pruebas terminadas");
    return 0;
}
