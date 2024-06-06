#include "http/HttpResponse.h"
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>

HttpResponse::HttpResponse(int status_code)
    : status_code(status_code) {
    // headers["Content-Type"] = "text/plain";
}

void HttpResponse::setHeader(const std::string& header_name, const std::string& header_value) {
    headers[header_name] = header_value;
}

std::string HttpResponse::getHeaders() const {
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " " << getStatusMessage() << "\r\n";
    for (const auto& header : headers) {
        response << header.first << ": " << header.second << "\r\n";
    }
    response << "\r\n";
    return response.str();
}

void HttpResponse::sendResponse(int client_socket, const std::string& body) const {
    std::string headers = getHeaders();
    send(client_socket, headers.c_str(), headers.size(), 0);
    if (!body.empty()) {
        send(client_socket, body.c_str(), body.size(), 0);
    }
}

void HttpResponse::sendChunkedResponse(int client_socket, std::ifstream& file) const {
    std::string headers = getHeaders();
    send(client_socket, headers.c_str(), headers.size(), 0);

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        std::ostringstream chunk_size;
        chunk_size << std::hex << file.gcount() << "\r\n";
        send(client_socket, chunk_size.str().c_str(), chunk_size.str().size(), 0);
        send(client_socket, buffer, file.gcount(), 0);
        send(client_socket, "\r\n", 2, 0);
    }

    std::string end_chunk = getChunkedTransferEncodingEnd();
    send(client_socket, end_chunk.c_str(), end_chunk.size(), 0);
}

std::string HttpResponse::getStatusMessage() const {
    switch (status_code) {
        case 200: return "OK";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        default: return "Unknown";
    }
}

std::string HttpResponse::getChunkedTransferEncodingEnd() const {
    return "0\r\n\r\n";
}
