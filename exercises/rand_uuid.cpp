#include <array>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <fmt/core.h>
#include <sodium.h>
#include <vector>

/*
 * - `randombytes_buf`
 * No genera números perśe, senera
 * entropía en bruto (bytes crudos (0–255))
 * sin estructura, y luego se decide
 * cómo interpretarla.
 *
 * - `randombytes_uniform`
 * números en rango [0, n) Sin sesgo,
 * a diferencia de módulos de `std::rand`
 * está basado en un CSPRNG interno, lo que
 * lo hace es seguro criptográficamente.
 *
 * - `UUID v4`
 * basado en bits aleatorios, no tiene orden
 * temporal, alta entropía.
 *
 * - `UUID v7`
 * basado en timestamp + entropía, ordenable por
 * tiempo de creación, diseñado para sistemas modernos.
 *
 * en el caso de uuid, si se desea un uso avanzado,
 * se recomienda utilizar librerías especificas
 * - stduuid (no v7 aún)
 * - libuuid(no v7 aún, solo linux)
 * - Boost.Uuid
 */

namespace naive_uuid {

    uint64_t unix_time_ms()
    {
        using namespace std::chrono;
        return static_cast<uint64_t>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    }

    std::string print_v7(const std::array<unsigned char, 16>& id)
    {
        return fmt::format("{:02x}{:02x}{:02x}{:02x}-"
                           "{:02x}{:02x}-"
                           "{:02x}{:02x}-"
                           "{:02x}{:02x}-"
                           "{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
                           id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8], id[9], id[10], id[11], id[12],
                           id[13], id[14], id[15]);
    }

    std::string print_v4(const std::array<unsigned char, 16>& id)
    {
        return fmt::format("{:02x}{:02x}{:02x}{:02x}-"
                           "{:02x}{:02x}-"
                           "{:02x}{:02x}-"
                           "{:02x}{:02x}-"
                           "{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
                           id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8], id[9], id[10], id[11], id[12],
                           id[13], id[14], id[15]);
    }

    std::string uuid_v4()
    {
        std::array<unsigned char, 16> id;

        randombytes_buf(id.data(), id.size());

        // version 4 -> xxxx0100
        id[6] = (id[6] & 0x0F) | 0x40;

        // variant RFC 4122 -> 10xxxxxx
        id[8] = (id[8] & 0x3F) | 0x80;

        return print_v4(id);
    }

    std::string uuid_v7()
    {
        std::array<unsigned char, 16> id;

        // 1. timestamp (48 bits)
        uint64_t ts = unix_time_ms();

        id[0] = (ts >> 40) & 0xFF;
        id[1] = (ts >> 32) & 0xFF;
        id[2] = (ts >> 24) & 0xFF;
        id[3] = (ts >> 16) & 0xFF;
        id[4] = (ts >> 8) & 0xFF;
        id[5] = ts & 0xFF;

        // 2. resto aleatorio
        randombytes_buf(id.data() + 6, 10);

        // 3. set version (v7 = 0x7)
        id[6] = (id[6] & 0x0F) | 0x70;

        // 4. set variant (RFC 4122)
        id[8] = (id[8] & 0x3F) | 0x80;

        return print_v7(id);
    }

} // namespace naive_uuid

int main()
{
    if (sodium_init() < 0) {
        return 1;
    }

    // uso de randombytes_uniform
    // upper_bound debe ser > 0
    const unsigned int upper_bound = 100;
    const int count = 10;

    std::vector<unsigned int> random_values;
    random_values.reserve(count);

    for (int i = 0; i < count; ++i) {
        unsigned int value = randombytes_uniform(upper_bound);
        random_values.push_back(value);
    }

    fmt::println("numeros generados:");

    for (unsigned int v : random_values) {
        fmt::print("{} ", v);
    }

    fmt::println("");

    // uso de randombytes_buf
    std::array<unsigned char, 16> buffer;

    randombytes_buf(buffer.data(), buffer.size());

    fmt::print("Bytes aleatorios: ");
    for (unsigned char b : buffer) {
        fmt::print("{:02x} ", b);
    }

    fmt::println("");

    // uso de uuid

    auto uidv4 = naive_uuid::uuid_v4();
    fmt::println("uuid v4: {}", uidv4);

    auto uidv7 = naive_uuid::uuid_v7();
    fmt::println("uuid v7: {}", uidv7);

    return 0;
}
