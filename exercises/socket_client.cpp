#include <arpa/inet.h>
#include <array>
#include <cstring>
#include <fmt/core.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr const char* ADDRESS = "127.0.0.1";
constexpr std::uint16_t PORT = 25575;

int main()
{
    fmt::println("Conectando al servidor TCP...");
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        fmt::println(stderr, "Error al conectar: socket() failed");
        return 1;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (::inet_pton(AF_INET, ADDRESS, &server.sin_addr) != 1) {
        fmt::println(stderr, "socket client Error: invalid address");
        ::close(sock);
        return 1;
    }

    if (::connect(sock, reinterpret_cast<sockaddr*>(&server), sizeof(server)) < 0) {
        fmt::println(stderr, "connect to server failed");
        ::close(sock);
        return 1;
    } else {
        fmt::println("Conectado al servidor...");
    }

    constexpr char msg[] = "hello from C++ client";

    auto sent = ::send(sock, msg, sizeof(msg) - 1, 0);

    if (sent < 0) {
        fmt::println(stderr, "send message failed");
    } else {
        fmt::println("sent message '{}'({} bytes)", msg, sent);
    }

    std::array<char, 1024> buffer{};

    auto bytes = ::recv(sock, buffer.data(), buffer.size(), 0);

    if (bytes > 0) {
        fmt::println("Respuesta del servidor: '{}'", std::string_view(buffer.data(), bytes));
    }

    fmt::println("cerrando cliente...");
    ::close(sock);
}
