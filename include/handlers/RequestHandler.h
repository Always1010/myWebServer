#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include <string>
#include <map>

class RequestHandler {
public:
    RequestHandler(const std::string& doc_root);
    void handleRequest(const HttpRequest& request, int client_socket);

private:
    std::string doc_root;
    static std::map<std::string, std::string> mime_types;
    static void initializeMimeTypes();

    void handleGetRequest(const HttpRequest& request, HttpResponse& response, int client_socket);
    std::string getMimeType(const std::string& path) const;
};

#endif // REQUESTHANDLER_H
