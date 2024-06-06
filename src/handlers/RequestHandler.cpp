#include "handlers/RequestHandler.h"
#include <fstream>
#include <sstream>
#include <filesystem>

RequestHandler::RequestHandler(const std::string& doc_root)
    : doc_root(doc_root) {
    // 初始化常见的 MIME 类型
    mime_types[".html"] = "text/html";
    mime_types[".css"] = "text/css";
    mime_types[".js"] = "application/javascript";
    mime_types[".png"] = "image/png";
    mime_types[".jpg"] = "image/jpeg";
    mime_types[".jpeg"] = "image/jpeg";
    mime_types[".gif"] = "image/gif";
    mime_types[".txt"] = "text/plain";
    mime_types[".json"] = "application/json";
    // 可以根据需要添加更多的 MIME 类型
}

void RequestHandler::handleRequest(const HttpRequest& request, int client_socket) {
    std::string method = request.getMethod();

    if (method == "GET") {
        HttpResponse response(200);
        handleGetRequest(request, response, client_socket);
    } else {
        HttpResponse response(405);
        response.setHeader("Content-Type", "text/plain");
        response.sendResponse(client_socket, "Method Not Allowed");
    }
}

void RequestHandler::handleGetRequest(const HttpRequest& request, HttpResponse& response, int client_socket) {
    std::string path = request.getPath();
    std::string file_path = doc_root + path;
    std::ifstream file(file_path, std::ios::binary);

    if (file.is_open()) {
        // 获取文件大小
        // std::filesystem::path fs_path(file_path);
        // auto file_size = std::filesystem::file_size(fs_path);
        std::string mime_type = getMimeType(file_path);
        response.setHeader("Content-Type", mime_type);
        // response.setHeader("Content-Length", std::to_string(file_size));
        response.setHeader("Transfer-Encoding", "chunked");
        response.sendChunkedResponse(client_socket, file);
    } else {
        response = HttpResponse(404);
        response.setHeader("Content-Type", "text/plain");
        response.sendResponse(client_socket, "File not found");
    }
}

std::string RequestHandler::getMimeType(const std::string& path) const {
    std::size_t dot_pos = path.find_last_of('.');
    if (dot_pos != std::string::npos) {
        std::string extension = path.substr(dot_pos);
        auto it = mime_types.find(extension);
        if (it != mime_types.end()) {
            return it->second;
        }
    }
    return "application/octet-stream"; // 默认二进制流类型
}
