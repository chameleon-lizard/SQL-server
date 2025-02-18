#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "include/parser.cpp"

#define PORT 8080

using namespace std;

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Receiving the query
    char buffer[1024] = {0};
    valread = read(new_socket, buffer, 1024);

    // Parsing the lexem
    std::vector<Lex::Lexem> lexems = Lex::parse(buffer);

    // Printing the query
    for (auto i = lexems.begin(); i != lexems.end(); i++)
    {
        std::cout << i->get_string() << " ";
    }
    std::cout << std::endl;

    // Analyzing, getting the answer and sending it back
    std::string answer = Parser::analyze(lexems);
    send(new_socket, answer.data(), answer.length(), 0);
    return 0;
}
