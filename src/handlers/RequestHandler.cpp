#include "handlers/RequestHandler.h"
#include <fstream>
#include <sstream>
#include <filesystem>

// 定义静态成员变量
std::map<std::string, std::string> RequestHandler::mime_types;

RequestHandler::RequestHandler(const std::string& doc_root, const std::string& default_route)
    : doc_root(doc_root), default_route(default_route) {
    // 如果 MIME 类型尚未初始化，则进行初始化
    if (mime_types.empty()) {
        initializeMimeTypes();
    }
}

void RequestHandler::initializeMimeTypes() {
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

    // 检查路径是否为根路径，如果是，则使用默认路由
    if (path == "/") {
        path = "/" + default_route;
    }

    std::string file_path = doc_root + path;
    std::ifstream file(file_path, std::ios::binary);

    if (file.is_open()) {
        std::string mime_type = getMimeType(file_path);
        response.setHeader("Content-Type", mime_type);
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
