#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>
#include <fstream>

class HttpResponse {
public:
    HttpResponse(int status_code);
    void setHeader(const std::string& header_name, const std::string& header_value);
    std::string getHeaders() const;
    void sendResponse(int client_socket, const std::string& body = "") const;
    void sendChunkedResponse(int client_socket, std::ifstream& file) const;

private:
    int status_code;
    std::map<std::string, std::string> headers;

    std::string getStatusMessage() const;
    std::string getChunkedTransferEncodingEnd() const;
};

#endif // HTTPRESPONSE_H
