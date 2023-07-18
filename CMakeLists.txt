cmake_minimum_required(VERSION 3.12)
project(SQL-proxy-server)

set(CMAKE_CXX_STANDARD 11)

# Установка стандартного пути установки библиотеки MySQL Connector
include_directories("C:\\Program Files\\MySQL\\MySQL Connector C++ 8.0\\include")
link_directories("C:\\Program Files\\MySQL\\MySQL Connector C++ 8.0\\lib64\\vs14")

add_executable(proxy-server 
            src/proxy-server.cpp 
            src/server-main.cpp 
            src/SQLhandler.cpp
            src/Logger.cpp)

# Для тестирования (2 клиента однвеременно)
add_executable(client 
            src/client-sock.cpp 
            src/client-main.cpp)
add_executable(client2 
            src/client-sock.cpp 
            src/client2-main.cpp)

# Линковка библиотеки MySQL Connector
target_link_libraries(proxy-server mysqlcppconn8)

# Скопировать динамическую библиотеку в build
add_custom_command(TARGET proxy-server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:\\Program Files\\MySQL\\MySQL Connector C++ 8.0\\lib64\\mysqlcppconn8-2-vs14.dll"
        $<TARGET_FILE_DIR:proxy-server>)