#include <array>
#include <cstring>
#include <fmt/core.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using socket_t = SOCKET;
constexpr socket_t INVALID_SOCKET_VAL = INVALID_SOCKET;
#
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
constexpr socket_t INVALID_SOCKET_VAL = -1;
#define SOCKET_ERROR -1
#define closesocket close
#
#endif

constexpr const char* ADDRESS = "127.0.0.1";
constexpr std::uint16_t PORT = 25575;

void cleanup_sockets()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fmt::println(stderr, "WSAStartup failed");
        return 1;
    }
#endif

    fmt::println("Conectando al servidor TCP...");
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        fmt::println(stderr, "Error al conectar: socket() failed");
        cleanup_sockets();
        return 1;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (::inet_pton(AF_INET, ADDRESS, &server.sin_addr) != 1) {
        fmt::println(stderr, "socket client Error: invalid address");
        //::close(sock);
        closesocket(sock);
        cleanup_sockets();
        return 1;
    }

    if (::connect(sock, reinterpret_cast<sockaddr*>(&server), sizeof(server)) < 0) {
        fmt::println(stderr, "connect to server failed");
        //::close(sock);
        closesocket(sock);
        cleanup_sockets();
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
    //::close(sock);
    closesocket(sock);
    cleanup_sockets();
}
