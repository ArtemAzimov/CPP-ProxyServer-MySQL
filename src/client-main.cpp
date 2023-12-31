#include "client-sock.hpp"
#include <vector>

constexpr char* mysql_user = "root";
constexpr char* mysql_password = "newpass";

int main() {
    // Создание сокета клиента и предоставление credentials к SQL Серверу
    Client myClient;
    myClient.connectToSocket(mysql_user, mysql_password);

    std::string query = "CREATE TABLE Customers (id INT PRIMARY KEY AUTO_INCREMENT"
    ",name VARCHAR(255), age INT, city VARCHAR(255));";
    myClient.createSQLRequest(query);

    query = "INSERT INTO Customers (name, age, city) VALUES ('Client', '26', 'New York');";
    myClient.createSQLRequest(query);

    query = "some incorrect user input";
    myClient.createSQLRequest(query);

    query = "SELECT * FROM Customers;";
    myClient.createSQLRequest(query);

    query = "INSERT INTO Customers (name, age, city) VALUES ('client1', '55', 'London');";
    // Множественные запросы
    for (int i = 0; i < 5000; ++i){
        myClient.createSQLRequest(query);
    }

    myClient.disconnect();

    return 0;
}