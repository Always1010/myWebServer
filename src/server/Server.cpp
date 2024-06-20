#include "server/Server.h"
#include "utils/Logger.h"
#include "http/HttpRequest.h"
#include "handlers/RequestHandler.h"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/select.h>

Server::Server(int port, const std::string& doc_root)
    : port(port), doc_root(doc_root), addrlen(sizeof(address)) {
    initServer();
}

void Server::initServer() {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        Logger::log("Socket creation error");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        Logger::log("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        Logger::log("Listen failed");
        exit(EXIT_FAILURE);
    }

    Logger::log("Server initialized on port " + std::to_string(port));
}

void Server::start() {
    Logger::log("Server started");

    fd_set master_set, working_set;
    FD_ZERO(&master_set);
    FD_SET(server_fd, &master_set);
    int max_sd = server_fd;

    while (true) {
        working_set = master_set;
        int activity = select(max_sd + 1, &working_set, nullptr, nullptr, nullptr);

        if (activity < 0 && errno != EINTR) {
            Logger::log("Select error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i <= max_sd; ++i) {
            if (FD_ISSET(i, &working_set)) {
                if (i == server_fd) {
                    int client_socket;
                    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                        Logger::log("Accept failed");
                        exit(EXIT_FAILURE);
                    }
                    Logger::log("Connection accepted");
                    FD_SET(client_socket, &master_set);
                    if (client_socket > max_sd) {
                        max_sd = client_socket;
                    }
                } else {
                    handleRequest(i);
                    close(i);
                    FD_CLR(i, &master_set);
                }
            }
        }
    }
}

void Server::handleRequest(int client_socket) {
    const int buffer_size = 30000;
    char buffer[buffer_size] = {0};
    read(client_socket, buffer, buffer_size);
    Logger::log("Request received:\n" + std::string(buffer));

    HttpRequest request(buffer);
    RequestHandler handler(doc_root);
    handler.handleRequest(request, client_socket);
}
