#include "Logger.hpp"

Logger::~Logger(){
    pLoggerSession->close();
    delete pLoggerSession;
};

void Logger::LoggerConnect(){
    try {
        pLoggerSession = new mysqlx::Session(
            SQL_SERVER_IP, SQL_SERVER_PORT, "root", "newpass", "logdb"
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

void Logger::writeQuery(const std::string query) {
    std::lock_guard<std::mutex> lock(logmx);
    //  "INSERT INTO testlog VALUES (\'" + query + "\');";

    std::string fullquery = "DECLARE @query NVARCHAR(MAX) = " + query + 
    " SET @query = CONCAT('INSERT INTO testlog VALUES (''', STRING_ESCAPE(@query, 'json'), ''');');" +
    "EXEC sp_executesql @query;";
    

    std::cout << "----fullquery " << fullquery << std::endl;
    try {
        pLoggerSession
        pLoggerSession->sql(fullquery).execute();
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