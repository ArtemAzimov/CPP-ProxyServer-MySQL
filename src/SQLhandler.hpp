#pragma once

#include "proxy-server.hpp"

// Подключается к MySQL серверу и передает ему SQL-запросы
class SQLhandler{
private:
    mysqlx::Session* pmySession = nullptr;
public:
    SQLhandler(){};

    // Конструктор для создания MySQL сессии с переданными параметрами
	SQLhandler(mysqlx::SessionSettings settings);
    
    ~SQLhandler();

	// Непосредственно взаимодействие с Database, вывод SQL Result
    void send_SQL_request(const std::string sql_request);

	// Клиент отключился - закрыть SQL сессию и освободить указатель
    void closeSession();
};