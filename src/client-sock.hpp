#pragma once

#include <winsock2.h>
#include <iostream>
#include <string>
#include <cstring>
#pragma comment(lib, "Ws2_32.lib") // Для линковки в MSVC

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

class Client{
private:
    // Инициализировать WinSock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET sock;

public:
    ~Client();

    int connectToSocket(const std::string user, const std::string password);

    int connectToSQL(const std::string credentials);

    int createSQLRequest(const std::string user_sql_request);

    void disconnect();
};