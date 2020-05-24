#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#define PORT 8080

using namespace std;

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Socket creation error" << std::endl;
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "Connection Failed" << std::endl;
        return 1;
    }

    std::string query = "CREATE TABLE test (First_name TEXT (10) , Age LONG, Phone TEXT (9) )";

    send(sock, query.data(), query.length(), 0);

    valread = read(sock, buffer, 1024);
    std::string answer(buffer);
    std::cout << answer << std::endl;

    return 0;
}
/*
Tests from book (and some more because there was no test for DELETE query, for instance). Just copy and paste to string and recompile the client.

"CREATE TABLE Students ( First_name TEXT ( 10 ) , Surname TEXT ( 15 ) , Age LONG , Phone TEXT ( 9 ) )";
"INSERT INTO Students ( 'Alexey' , 'Petrov' , 20 , '343-65-45' )";
"SELECT * FROM Students WHERE ALL";
"DELETE FROM Students WHERE First_name = 'Alexey'";
"SELECT * FROM Students WHERE ALL";
"INSERT INTO Students ( 'Alexey' , 'Petrov' , 20 , '343-65-45' )";
"INSERT INTO Students ( 'Andrey' , 'Fedorov' , 23 , '123-45-18' )";
"INSERT INTO Students ( 'Alexandre' , 'Zaharov' , 20 , '345-33-33' )";
"SELECT * FROM Students WHERE ALL";
"SELECT First_name , Surname FROM Students WHERE Age IN ( 18 , 19 , 20 ) ";
"SELECT * FROM Students WHERE Phone LIKE '%-%-45'";
"SELECT * FROM Students WHERE Phone LIKE '%45%'";
"SELECT Phone FROM Students WHERE Surname = 'Petrov'";
"SELECT Surname FROM Students WHERE ALL";
"SELECT * FROM Students WHERE First_name LIKE '[ABC][^mno]_x%'";
"SELECT Surname , Phone FROM Students WHERE ( Age > 19 ) AND ( Surname > 'M' )";
"SELECT * FROM Students WHERE ALL";
"UPDATE Students SET Age = Age + 1 WHERE ALL";
"SELECT * FROM Students WHERE ALL";
"DROP TABLE Students";
*/