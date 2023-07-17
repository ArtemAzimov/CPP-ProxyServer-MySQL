#pragma once

#include "client-sock.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
// Для линковки в MSVS
#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

// Инициализировать WinSock
WSADATA wsaData;
int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
SOCKET sock;

Client::~Client(){
    closesocket(sock);
    WSACleanup();
}

int Client::connectToSocket(const std::string user, const std::string password){
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Создать сокет
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Could not create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Указать данные сокета для подключения
    sockaddr_in serverAddress{};
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);

    // Подключиться к прокси-серверу
    if (connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    std::cout << "Connected successfully to proxy-server" << std::endl;
    std::string credent = user+' '+password;
    connectToSQL(credent);
}

int Client::connectToSQL(const std::string credentials){
    std::cout << "Connecting to sql..." << std::endl;
    // Отправить запрос прокси-серверу
    iResult = send(sock, credentials.c_str(), strlen(credentials.c_str()), 0);
    if (iResult == -1) {
        std::cerr << "Error when sending data to server: " << strerror(errno) << std::endl;
        closesocket(sock);
        return 1;
    }

    // Выделить память под сообщение от прокси-сервера
    char response[BUFFER_SIZE];
    memset(response, 0, sizeof(response));

    // Получить ответ от прокси-сервера
    size_t bytesRead = recv(sock, response, sizeof(response) - 1, 0);
    if (bytesRead == -1) {
        std::cerr << "Error when reading from server: " << strerror(errno) << std::endl;
        closesocket(sock);
        return 1;
    }
    std::cout << "Client connection status: " << response << std::endl;
}

int Client::createSQLRequest(const std::string user_sql_request){
    // Отправить запрос прокси-серверу
    iResult = send(sock, user_sql_request.c_str(), strlen(user_sql_request.c_str()), 0);
    if (iResult == -1) {
        std::cerr << "Error when sending data to server: " << strerror(errno) << std::endl;
        closesocket(sock);
        return 1;
    }

    // Выделить память под сообщение от прокси-сервера
    char response[BUFFER_SIZE];
    memset(response, 0, sizeof(response));

    // Получить ответ от прокси-сервера
    size_t bytesRead = recv(sock, response, sizeof(response) - 1, 0);
    if (bytesRead == -1) {
        std::cerr << "Error when reading from server: " << strerror(errno) << std::endl;
        closesocket(sock);
        return 1;
    }
    std::cout << "Query in process: " << response << std::endl;
}

void Client::disconnect(){
    Client::~Client();
}