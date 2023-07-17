#include "client-sock.hpp"
#include <vector>

int main() {
    // std::string userinput;
    // std::getline(std::cin, userinput);

    const std::string mysql_user = "root";
    const std::string mysql_password = "newpass";

    Client myClient;
    myClient.connectToSocket(mysql_user, mysql_password);

    std::string query = "CREATE TABLE Customers (id INT PRIMARY KEY AUTO_INCREMENT"
    ",name VARCHAR(50), age INT, city VARCHAR(100));";
    myClient.createSQLRequest(query);

    query = "INSERT INTO Customers (name, age, city) VALUES ('Artem', '26', 'Kazan');";
    myClient.createSQLRequest(query);

    query = "some incorrect user input";
    myClient.createSQLRequest(query);

    query = "SELECT * FROM Customers;";
    myClient.createSQLRequest(query);

    myClient.disconnect();

    return 0;
}