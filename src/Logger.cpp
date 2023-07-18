#include "Logger.hpp"

Logger::~Logger(){
    pLoggerSession->close();
    delete pLoggerSession;
};

void Logger::LoggerConnect(){
    try {
        pLoggerSession = new mysqlx::Session(
            LOGGER_SERVER_IP, LOGGER_SERVER_PORT, LOGGER_USER, LOGGER_PASSWORD, LOGGER_DATABASE
            );
        std::cout << "Logger connected successfully to MySQL Server" << std::endl;
    } catch (mysqlx::Error& e) {
        std::cerr << "Error(Logger): " << e.what() << std::endl;
        closeSession();
    } catch (...) {
        std::cout << "Error(Logger): Something went wrong" << std::endl;
        closeSession();
    }
}

void Logger::writeQuery(std::string query) {
    std::lock_guard<std::mutex> lock(logmx);

    // Экранирование запроса при записи в лог
    std::string escapedQuery;
    for (int i = 0; i < query.size(); ++i){
        if (query[i] == '\''){
            escapedQuery.append("\'\'");
        }
        else{
            escapedQuery.push_back(query[i]);
        }
    }
    std::string fullQuery = "INSERT INTO testlog VALUES (\'" + escapedQuery + "\');";
    std::cout << "----fullQuery: " << fullQuery << std::endl;

    try {
        pLoggerSession->sql(fullQuery).execute();
        std::cout << "----Logger SUCCESS" << std::endl;
    } 
    catch (mysqlx::Error& e) {
        std::cerr << "Error(Logger): " << e.what() << std::endl;
        // closeSession();
    } 
    catch (...) {
        std::cout << "Error(Logger): Something went wrong" << std::endl;
        closeSession();
    }
}