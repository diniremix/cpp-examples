#include <array>
#include <cstring>
#include <fmt/core.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

using socket_t = SOCKET;
constexpr socket_t INVALID_SOCKET_VAL = INVALID_SOCKET;
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>

    #define SOCKET_ERROR -1
    #define closesocket close

using socket_t = int;
constexpr socket_t INVALID_SOCKET_VAL = -1;
#endif

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

    socket_t server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET_VAL) {
        fmt::println(stderr, "Error al conectar: socket() failed");
        cleanup_sockets();
        return 1;
    }

    // Configurar opción para reutilizar dirección (útil en Linux/Unix)
#ifndef _WIN32
    int opt = 1;
    if (::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        fmt::println(stderr, "Error al conectar: setsockopt() failed");
    }
#endif

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (::bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        fmt::println(stderr, "server internal error: bind() failed");
        closesocket(server_fd);
        cleanup_sockets();
        return 1;
    }

    if (::listen(server_fd, SOMAXCONN) < 0) {
        fmt::println(stderr, "server internal error: listen() failed");
        closesocket(server_fd);
        cleanup_sockets();
        return 1;
    }

    fmt::println("Servidor TCP escuchando en puerto {}...", PORT);

    // utiliza un ciclo while, para aceptar múltiples conexiones
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        socket_t client_fd = ::accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);

        if (client_fd == INVALID_SOCKET_VAL) {
            fmt::println(stderr, "Error al aceptar conexión: accept() failed...");
            continue;
        }

        // Obtener IP del cliente
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        fmt::println("Cliente conectado desde {}:{}", client_ip, ntohs(client_addr.sin_port));

        std::array<char, 1024> buffer{};
        auto bytes = ::recv(client_fd, buffer.data(), buffer.size(), 0);

        if (bytes > 0) {
            fmt::println("mensaje recibido: '{}'({} bytes)", std::string_view(buffer.data(), bytes), bytes);

            fmt::println("enviando respuesta al cliente...");
            constexpr const char* response = "Hola desde el servidor TCP";
            ::send(client_fd, response, std::strlen(response), 0);
        } else if (bytes == 0) {
            fmt::println("Cliente cerró la conexión");
        } else {
            fmt::println(stderr, "Error al recibir datos");
        }

        closesocket(client_fd);
        fmt::println("Conexión con cliente cerrada\n");
    }

    closesocket(server_fd);
    cleanup_sockets();

    return 0;
}
