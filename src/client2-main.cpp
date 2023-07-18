#include "client-sock.hpp"
#include <vector>

constexpr char* mysql_user = "root";
constexpr char* mysql_password = "newpass";

int main() {
    // Создание сокета клиента и предоставление credentials к SQL Серверу
    Client myClient;
    myClient.connectToSocket(mysql_user, mysql_password);

    std::string query = "CREATE TABLE Students (id INT PRIMARY KEY AUTO_INCREMENT"
    ",name VARCHAR(255), age INT, city VARCHAR(255));";
    myClient.createSQLRequest(query);

    query = "INSERT INTO Students (name, age, city) VALUES ('Client2', '26', 'New York');";
    myClient.createSQLRequest(query);

    query = "some incorrect user input";
    myClient.createSQLRequest(query);

    query = "SELECT * FROM Students;";
    myClient.createSQLRequest(query);

    query = "INSERT INTO Students (name, age, city) VALUES ('Client2', '55', 'London');";
      // Множественные запросы
    for (int i = 0; i < 5000; ++i){
        myClient.createSQLRequest(query);
    }

    myClient.disconnect();

    return 0;
}