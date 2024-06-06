#include "server/Server.h"
#include "utils/Config.h"
#include "utils/Logger.h"

int main() {
    try {
        Config config("../server.config");
        int port = std::stoi(config.get("port"));
        std::string doc_root = config.getAbsolutePath("doc_root");
        
        Logger::log("Server starting on port: " + std::to_string(port));
        Logger::log("Document root: " + doc_root);

        Server server(port, doc_root);
        server.start();
    } catch (const std::exception& ex) {
        Logger::log(std::string("Error: ") + ex.what());
        return 1;
    }
    return 0;
}
