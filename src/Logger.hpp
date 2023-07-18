#pragma once

#include <mutex>
#include "proxy-server.hpp"

class Logger{
private:
    mysqlx::Session* pLoggerSession = nullptr;
	std::mutex logmx;

private:
	Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator = (const Logger&) = delete;

public:
	~Logger();
	static Logger& self(){
		static Logger instance;
		return (instance);
	}

    void LoggerConnect();

    void writeQuery(const std::string query);

	// Клиент отключился - закрыть SQL сессию и освободить указатель
    void closeSession() {
        pLoggerSession->close();
		delete pLoggerSession;
    }
};