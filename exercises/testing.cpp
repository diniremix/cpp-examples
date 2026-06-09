#include <algorithm>
#include <cassert>
#include <fmt/core.h>
#include <stdexcept>
#include <type_traits>
#include <vector>

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
    return static_cast<float>(a) / b;
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
    return static_cast<float>(suma) / datos.size();
}

int main()
{
    test_divide();
    test_maximo();

    fmt::println("todas las pruebas pasaron");

    // demostración de error
    float resultado = divide(10, 3);
    fmt::println("10 / 3 = {:.2f}", resultado);

    std::vector<int> numeros = {10, 20, 30, 40};
    float media = calcular_media(numeros);
    fmt::println("media: {:.2f}", media);

    std::vector<int> vacio;
    calcular_media(vacio); // ❌ assert falla
    // mensaje: Assertion `!datos.empty() && "no se puede calcular media de vector vacío"' failed

    int numero = 10;

    // caso 1: correcto
    procesar(&numero, 42);
    fmt::println("numero: {}", numero); // numero: 42

    // caso 2: valor negativo (falla assert)
    procesar(&numero, -5); // ❌ assertion: valor > 0

    static_assert(sizeof(int) == 4, "int debe ser 4 bytes");
    static_assert(1 + 1 == 2, "matemáticas básicas fallaron");

    dividir(10, 2); // OK
    // dividir<std::string>("a", "b");  // ❌ error en compilación

    fmt::println("compilación exitosa");

    return 0;
}
