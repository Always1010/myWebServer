#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <netinet/in.h>
#include "threadpool/ThreadPool.h"

class Server {
public:
    Server(int port, const std::string& doc_root, size_t thread_pool_size);
    void start();

private:
    int server_fd;
    int port;
    std::string doc_root;
    struct sockaddr_in address;
    int addrlen;

    ThreadPool thread_pool;

    void initServer();
    void acceptConnection();
    void handleRequest(int client_socket);
};

#endif // SERVER_H
