#include "http/HttpRequest.h"
#include <sstream>
#include <algorithm>

HttpRequest::HttpRequest(const std::string& raw_request) {
    parseRequest(raw_request);
}

std::string HttpRequest::getMethod() const {
    return method;
}

std::string HttpRequest::getPath() const {
    return path;
}

std::string HttpRequest::getVersion() const {
    return version;
}

std::string HttpRequest::getHeader(const std::string& header_name) const {
    auto it = headers.find(header_name);
    if (it != headers.end()) {
        return it->second;
    }
    return "";
}

std::string HttpRequest::getBody() const {
    return body;
}

void HttpRequest::parseRequest(const std::string& raw_request) {
    std::istringstream request_stream(raw_request);
    std::string line;

    // Parse request line
    std::getline(request_stream, line);
    std::istringstream request_line(line);
    request_line >> method >> path >> version;

    // Parse headers
    while (std::getline(request_stream, line) && line != "\r") {
        auto colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string header_name = line.substr(0, colon_pos);
            std::string header_value = line.substr(colon_pos + 1);
            // Remove whitespace
            header_value.erase(header_value.begin(), std::find_if(header_value.begin(), header_value.end(), [](int ch) {
                return !std::isspace(ch);
            }));
            header_value.erase(std::find_if(header_value.rbegin(), header_value.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), header_value.end());
            headers[header_name] = header_value;
        }
    }

    // Parse body
    if (request_stream.good()) {
        std::ostringstream body_stream;
        body_stream << request_stream.rdbuf();
        body = body_stream.str();
    }
}
