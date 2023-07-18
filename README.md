## Description
- Все данные для подключений, а так же имена баз данных заданы constexpr в "proxy-server.hpp"
- Данные клиентов (тестовые) заданы constexpr в "client-main.cpp"

## Environment
- Тестировалось на Windows 10 x64, генератор Visual Studio 17 2022, стандарт C++11
- CMake version 3.27.0-rc4
- MySQL Community Server 8.0
- MySQL Connector C++ 8.0 X DevAPI (динамическая библиотека)

## Build commands
- cmake -S . -B ./build/
- cmake --build ./build --config Release

## Run commands
- ./build/Release/proxy-server.exe
- ./build/Relese/client.exe
- ./build/Relese/client2.exe
