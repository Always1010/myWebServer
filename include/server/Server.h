#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <string>

class Server {
public:
    Server(int port, const std::string& doc_root);
    void start();

private:
    int server_fd;
    int port;
    std::string doc_root;
    struct sockaddr_in address;
    int addrlen;

    void initServer();
    void acceptConnection();
    void handleRequest(int client_socket);
};

#endif // SERVER_H
