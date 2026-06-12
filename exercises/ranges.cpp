#include <algorithm>
#include <fmt/core.h>
#include <ranges>
#include <string>
#include <vector>

/*
 * ranges son para describir cómo recorrer y
 * transformar datos, no para almacenarlos.
 * recuerda que las 'views/vistas' no poseen los datos
 *
 * - Un rango (range) es algo que puede recorrerse
 *
 * - Una vista (view) NO almacena datos.
 * Solo describe cómo acceder o transformar eso datos
 *
 * - Las views suelen ser `lazy`:
 * el trabajo se realiza cuando alguien consume la vista
 *
 * - Las views normalmente dependen del rango original.
 * Si el rango deja de existir, la vista puede quedar
 * inválida (comportamiento indefinido)
 *
 * en resumen:
 * Una view es más parecida a una consulta SQL
 * `SELECT ... WHERE ...`
 * que a un `std::vector`
 * ya que describe cómo obtener resultados,
 * pero normalmente no contiene resultados
 *
 * - Cuando se necesita un resultado independiente,
 * la vista debe materializarse en un contenedor
 * (por ejemplo std::vector).
 *
 * std::vector -> posee datos
 * std::views -> observa datos
 *
 * Los ranges no reemplazan a los contenedores.
 * trabajan sobre ellos.
 *
 */

/* sin ranges:
 * recorrer el vector de números
 * obtener numeros pares
 * elevar al cuadrado cada valor encontrado
 * guardar resultado en 'resultado'
 * devolver 'resultado'
 */
std::vector<int> pares_cuadrados(std::vector<int>& numeros)
{
    std::vector<int> resultado;

    for (int n : numeros) {
        if (n % 2 == 0) {
            resultado.push_back(n * n);
        }
    }
    return resultado;
}

/* con ranges
 * filtrar pares
 * transformar a cuadrados
 * devolver 'resultado'
 *
 * Pipeline: filter -> transform
 */
std::vector<int> pares_cuadrados_ranges(std::vector<int>& numeros)
{
    // crea una 'vista' que apunta a los resultados
    auto view_result = numeros | std::views::filter([](int n) { return n % 2 == 0; }) // pares
                       | std::views::transform([](int n) { return n * n; });          // cuadrados

    // materializar la 'vista'
    return {view_result.begin(), view_result.end()};
}

// devuelve la view(vista), no los datos
auto pares_cuadrados_ranges_view(const std::vector<int>& numeros)
{
    auto view_result = numeros | std::views::filter([](int n) { return n % 2 == 0; }) |
                       std::views::transform([](int n) { return n * n; });

    return view_result;
}

// devuelve la view(vista), no los datos
auto procesar_nombres(std::vector<std::string>& name_list)
{
    auto long_names = name_list | std::views::filter([](const std::string& s) { return s.size() >= 3; }) |
                      std::views::transform([](const std::string& s) {
                          std::string upper = s;
                          // std::ranges::for_each(upper, [](char& c) { c = std::toupper(c); });

                          for (char& c : upper) {
                              c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
                          }

                          return upper;
                      });

    return long_names;
}

int main()
{
    std::vector<int> numeros{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto result1 = pares_cuadrados(numeros);

    fmt::println("pares_cuadrados");
    for (int val : result1) {
        fmt::print("{} ", val); // 4 16 36
    }
    fmt::println("");

    fmt::println("pares_cuadrados(ranges)");

    auto result2 = pares_cuadrados_ranges(numeros);

    for (int val : result2) {
        fmt::print("{} ", val); // 4 16 36
    }

    fmt::println("");
    fmt::println("pares_cuadrados(ranges view)");

    // esto funciona ya que el rango original (el vector `numeros`)
    // existe en `main()`.
    auto result3 = pares_cuadrados_ranges_view(numeros);

    for (int val : result3) {
        fmt::print("{} ", val); // 4 16 36
    }

    fmt::println("");
    fmt::println("procesar strings");

    // procesar strings
    std::vector<std::string> nombres{"alice", "bob", "charlie", "david", "ed"};

    fmt::print("Nombres largos en mayúsculas: ");

    // esto funciona ya que el rango original (el vector `nombres`)
    // existe en `main()`.
    auto result_names = procesar_nombres(nombres);

    for (const auto& nombre : result_names) {
        fmt::print("{} ", nombre);
    }

    fmt::println("");

    fmt::print("Primeros 5 cuadrados: ");

    /*
     * Generar secuencias infinitas (lazy!)
     * aqui usamos `iota` que puede generar secuencias infinitas
     * asi que conviene limitar el recorrido con vistas como: `take(N)`
     * o consumir solo una cantidad finita de elementos.
     */
    auto cuadrados_infinitos = std::views::iota(1) // 1, 2, 3, 4...
                               | std::views::transform([](int n) { return n * n; });

    for (int val : cuadrados_infinitos | std::views::take(10)) {
        fmt::print("{} ", val); // 1 4 9 16 25 36 49 64 81 100
    }

    fmt::println("");

    return 0;
}
