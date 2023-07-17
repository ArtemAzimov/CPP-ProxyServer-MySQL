#include "SQLhandler.hpp"
#include "Logger.hpp"

SQLhandler::~SQLhandler(){
    pmySession->close();
    delete pmySession;
};

// Конструктор для создания MySQL сессии с переданными параметрами
SQLhandler::SQLhandler(mysqlx::SessionSettings settings){
    try {
        pmySession = new mysqlx::Session(settings);
        std::cout << "Proxy-server connected successfully to MySQL Server" << std::endl;
    } catch (mysqlx::Error& e) {
        std::cerr << "Error(sqlSession): " << e.what() << std::endl;
        closeSession();
    } catch (...) {
        std::cout << "Error(sqlSession): Something went wrong" << std::endl;
        closeSession();
    }
}

// Непосредственно взаимодействие с Database, вывод SQL Result
void SQLhandler::send_SQL_request(const std::string sql_request) {
    try {
        auto result = pmySession->sql(sql_request).execute();
        std::cout << "----Query SUCCESS" << std::endl;
        Logger::self().writeQuery(sql_request);
    } 
    catch (mysqlx::Error& e) {
        std::cerr << "Error(sqlQuery): " << e.what() << std::endl;
        // closeSession();
    } 
    catch (...) {
        std::cout << "Error(sqlQuery): Something went wrong" << std::endl;
        closeSession();
    }
}

void SQLhandler::closeSession(){
    SQLhandler::~SQLhandler();
};