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

Server::Server(int port, const std::string& doc_root, const std::string& default_route, size_t thread_pool_size)
    : port(port), doc_root(doc_root), default_route(default_route), addrlen(sizeof(address)), thread_pool(thread_pool_size) {
    initServer();
}

void Server::initServer() {
    // 创建 socket 文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        Logger::log("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // 绑定地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        Logger::log("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听端口
    if (listen(server_fd, 1024) < 0) {
        Logger::log("Listen failed");
        exit(EXIT_FAILURE);
    }

    Logger::log("Server initialized on port " + std::to_string(port));
}

void Server::start() {
    Logger::log("Server started");
    while (true) {
        acceptConnection();
    }
}

void Server::acceptConnection() {
    int client_socket;
    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        Logger::log("Accept failed");
        return;
    }
    Logger::log("Connection accepted");

    // 将请求处理任务添加到线程池中
    thread_pool.enqueue(&Server::handleRequest, this, client_socket);
}

void Server::handleRequest(int client_socket) {
    const int buffer_size = 30000;
    char buffer[buffer_size] = {0};
    read(client_socket, buffer, buffer_size);
    Logger::log("Request received:\n" + std::string(buffer));

    HttpRequest request(buffer);
    RequestHandler handler(doc_root,default_route);
    handler.handleRequest(request, client_socket);
    close(client_socket);
}
