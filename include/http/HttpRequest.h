#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <map>

class HttpRequest {
public:
    HttpRequest(const std::string& raw_request);

    std::string getMethod() const;
    std::string getPath() const;
    std::string getVersion() const;
    std::string getHeader(const std::string& header_name) const;
    std::string getBody() const;

private:
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;

    void parseRequest(const std::string& raw_request);
};

#endif // HTTPREQUEST_H
