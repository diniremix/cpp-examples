#include <filesystem>
#include <fmt/core.h>
#include <fstream> // Para ifstream, ofstream, fstream
#include <iostream>

namespace fs = std::filesystem;

// https://www.geeksforgeeks.org/cpp/pod-type-in-cpp/

struct PersonPOD {
    char name[100]; // Arreglo fijo en lugar de string
    std::uint32_t age;
    bool edge;
};

int main()
{
    fmt::println("manejo de archivos binarios (POD types)");

    fs::path file_path = "person_pod_example.bin";

    fmt::println("usando POD(Plain Old Data)");
    PersonPOD person_pod{"Peter", 456, true};
    fmt::println("name={}, age={}, edge={}", person_pod.name, person_pod.age, person_pod.edge);
    fmt::println("");

    // crear el archivo como binario
    std::ofstream outFile(file_path, std::ios::binary);
    if (!outFile) {
        fmt::println(stderr, "Error opening the file for writing.");
        return 1;
    }

    // convertir el struct a bytes
    // y guardar los datos
    outFile.write(reinterpret_cast<const char*>(&person_pod), sizeof(person_pod));
    outFile.close();

    // Leer bytes del archivo
    PersonPOD bin_data{};

    std::ifstream inFile(file_path, std::ios::binary);
    if (!inFile) {
        fmt::println(stderr, "Error opening the file for reading");
        return 1;
    }

    inFile.read(reinterpret_cast<char*>(&bin_data), sizeof(bin_data));

    fmt::println("datos leídos: name={}, age={}, edge={}", bin_data.name, bin_data.age, bin_data.edge);
    inFile.close();

    return 0;
}
