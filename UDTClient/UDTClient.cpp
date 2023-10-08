#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    if (WSAStartup(ver, &wsData) != 0) {
        std::cerr << "Can't initialize winsock" << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Can't create socket" << std::endl;
        return 1;
    }

    // Set up the server address
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(54000);  // Порт сервера
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);  // IP-адрес сервера (в данном случае, localhost)

    std::string userMessage;
    while (true) {
        std::cout << "Enter a message: ";
        std::getline(std::cin, userMessage);

        // Send the message
        sendto(udpSocket, userMessage.c_str(), userMessage.size() + 1, 0, (sockaddr*)&server, sizeof(server));

        // Receive response
        char buf[1024];
        ZeroMemory(buf, 1024);
        int bytesIn = recv(udpSocket, buf, 1024, 0);
        if (bytesIn == SOCKET_ERROR) {
            std::cerr << "Error receiving from server" << std::endl;
            continue;
        }

        std::cout << "Server: " << std::string(buf, 0, bytesIn) << std::endl;
    }

    // Close socket
    closesocket(udpSocket);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}
