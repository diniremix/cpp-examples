#include <filesystem>
#include <fmt/core.h>
#include <fstream> // Para ifstream, ofstream, fstream
#include <iostream>
#include <optional>
#include <string>
#include <vector>

/*
 * tipos complejos (non-pod) requieren serialización manual.
 * se necesita serialización/deserialización componente por componente.
 *
 * El orden de escritura y lectura DEBE coincidir exactamente.
 * para casos mas avanzados se recomienda librerías de serialización binaria
 * - cereal
 * - Bitsery
 * - MsgPack
 * - CBOR
 * - Cap'n Proto
 * - protobuf
 * - Boost.Serialization
 *
 * ejemplo: Esto NO funciona:
 *
 * `std::string name;`
 * `outFile.write(reinterpret_cast<const char*>(&name), sizeof(name));`
 *
 * debido a que...
 * `std::string` es un tipo complejo que maneja memoria dinámica
 * cuando se hace `sizeof(name)` se obtiene el tamaño del objeto
 * en el stack (normalmente 32 bytes para un string + 4 para el int),
 * y no el tamaño completo del string, incluyendo su contenido en el heap.
 * asi que...
 * - solo escribe los bytes del objeto en el STACK (punteros internos del string)
 * - los datos reales del string (en HEAP) no se escriben
 * - al leer del archivo, los punteros apuntan a memoria inválida
 */

namespace fs = std::filesystem;

struct Person {
    std::string name;
    std::uint32_t age;
    std::optional<std::vector<std::string>> interests;

    // se utiliza un buffer contiguo
    // para serializacion de datos
    std::vector<char> save_to_bytes() const
    {
        std::vector<char> buffer;

        // 1. Guardar nombre (tamaño)
        uint32_t name_len = name.size();
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&name_len),
                      reinterpret_cast<const char*>(&name_len) + sizeof(name_len));
        // 1.1 Guardar nombre (datos)
        buffer.insert(buffer.end(), name.begin(), name.end());

        // 2. Guardar edad
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&age),
                      reinterpret_cast<const char*>(&age) + sizeof(age));

        // 3. validar interests (campo opcional)
        // 3.1 almacenar estado
        bool has_interests = interests.has_value();
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&has_interests),
                      reinterpret_cast<const char*>(&has_interests) + sizeof(has_interests));

        // 3.2 almacenar si tiene valores
        if (has_interests) {
            uint32_t num_interests = interests->size();
            // cantidad de elementos
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&num_interests),
                          reinterpret_cast<const char*>(&num_interests) + sizeof(num_interests));

            // recorrer elementos
            for (const auto& interest : *interests) {
                // almacenar el tamaño de cada elemento
                uint32_t interest_len = interest.size();
                buffer.insert(buffer.end(), reinterpret_cast<char*>(&interest_len),
                              reinterpret_cast<char*>(&interest_len) + sizeof(interest_len));
                // almacenara el valor de cada elemento
                buffer.insert(buffer.end(), interest.begin(), interest.end());
            }
        }

        return buffer;
    }

    // Leer desde buffer contiguo
    // para deserializacion de datos
    static Person load_from_bytes(const std::vector<char>& buffer)
    {
        Person p;
        size_t pos = 0;

        // 1. Leer el tamaño del campo nombre
        uint32_t name_len = *reinterpret_cast<const uint32_t*>(&buffer[pos]);
        pos += sizeof(name_len);

        // 1.1 leer el valor del campo nombre
        p.name.assign(&buffer[pos], name_len);
        pos += name_len;

        // 2. leer el campo edad
        p.age = *reinterpret_cast<const uint32_t*>(&buffer[pos]);
        pos += sizeof(p.age);

        // 3. leer el campo interests
        // este campo al ser un array se debe determinar
        // 3.1 si tiene elementos
        bool has_interests = *reinterpret_cast<const bool*>(&buffer[pos]);
        pos += sizeof(has_interests);

        // 3.2 obtener cada uno de ellos
        if (has_interests) {
            // cantidad de elementos
            uint32_t num_interests = *reinterpret_cast<const uint32_t*>(&buffer[pos]);
            pos += sizeof(num_interests);

            std::vector<std::string> interests_vec;
            interests_vec.reserve(num_interests);

            // recorrer la lista de de elementos
            for (uint32_t i = 0; i < num_interests; ++i) {
                uint32_t interest_len = *reinterpret_cast<const uint32_t*>(&buffer[pos]);
                pos += sizeof(interest_len);

                std::string interest(&buffer[pos], interest_len);
                pos += interest_len;

                interests_vec.push_back(std::move(interest));
            }

            p.interests = std::move(interests_vec);
        }

        return p;
    }
};

void save_bin_file(Person& person, const fs::path& file_path)
{
    fmt::println("creando archivo: '{}'", file_path.string());

    auto buffer = person.save_to_bytes();

    // crear el archivo
    std::ofstream outFile(file_path, std::ios::binary);
    outFile.write(buffer.data(), buffer.size());
    outFile.close();

    fmt::println("Tamaño: {} bytes", fs::file_size(file_path));
}

void load_bin_file(const fs::path& file_path)
{
    fmt::println("leyendo archivo: '{}'", file_path.string());

    // leer todo el archivo
    std::ifstream inFile(file_path, std::ios::binary);
    std::vector<char> read_buffer(std::istreambuf_iterator<char>(inFile), {});

    // Deserializar
    auto bin_data = Person::load_from_bytes(read_buffer);
    inFile.close();

    fmt::println("");
    fmt::println("datos leídos: name={}, age={}", bin_data.name, bin_data.age);

    if (bin_data.interests) {
        fmt::println("interests:");
        for (const auto& str : bin_data.interests.value()) {
            fmt::println(" - {}", str); // Imprimir cada string
        }
    }
}

int main()
{
    fmt::println("manejo de archivos binarios (non-POD types)");

    fs::path file_path = "data1_example.bin";

    // 1st
    Person alice{"Alice", 28, {{"coding", "reading", "beer"}}};
    fmt::println("");
    save_bin_file(alice, file_path);

    fmt::println("");
    load_bin_file(file_path);

    // 2nd
    file_path = "data2_example.bin";

    Person jane{"Jane", 31, std::nullopt};
    fmt::println("");
    save_bin_file(jane, file_path);

    fmt::println("");
    load_bin_file(file_path);

    return 0;
}
