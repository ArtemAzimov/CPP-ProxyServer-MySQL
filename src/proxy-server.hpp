#pragma once

#include <mysqlx/xdevapi.h>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <thread>
#include <stdexcept>
#include <vector>
#include <mutex>
#include "SQLhandler.hpp"
#include "Logger.hpp"
#pragma comment(lib, "Ws2_32.lib") // Для линковки в MSVC

// defaults for proxy-server connection
constexpr char* SERVER_IP = "127.0.0.1";
constexpr unsigned SERVER_PORT = 8080;
constexpr size_t BUFFER_SIZE = 1024;

// defaults for mysql server connection
constexpr char* SQL_SERVER_IP = "127.0.0.1";
constexpr unsigned SQL_SERVER_PORT = 33060;
constexpr char* SQL_DATABASE = "testdb";

// default for logger connection
constexpr char* LOGGER_SERVER_IP = "127.0.0.1";
constexpr unsigned LOGGER_SERVER_PORT = 33060;
constexpr char* LOGGER_USER = "root";
constexpr char* LOGGER_PASSWORD = "newpass";
constexpr char* LOGGER_DATABASE = "logdb";


class SQLhandler;
class ProxyServer{
private:
    // Инициализация WinSocket
    WSADATA wsaData;
	SOCKET sock;
	// Вектор потоков подключений клиентов
	std::vector<std::thread> vecThread;

	// Отправка sql-query SQL серверу и вывод результата клиенту
	void sendQuery(int clientSock, SQLhandler* pSQLhandle);

	// Обработка сервером запроса клиента
	void handleClient(int clientSock);

public:
    ~ProxyServer();

	// Детали сессии для SQL сервера
	mysqlx::SessionSettings setSessionSettings(const std::string host, unsigned port, 
									const std::string user, const char* pwd, const std::string db);
	
    // Запуск прокси-сервера
	int run();

	// Дисконнект сокета
	void shutdown();
};