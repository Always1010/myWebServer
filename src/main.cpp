#include "server/Server.h"
#include "utils/Config.h"
#include "utils/Logger.h"

int main() {
    try {
        // 加载配置文件
        Config config("../server.config");
        int port = std::stoi(config.get("port")); // 从配置文件中获取端口号
        std::string doc_root = config.getAbsolutePath("doc_root"); // 获取文档根目录的绝对路径
        size_t thread_pool_size = std::stoi(config.get("thread_pool_size"));

        // 记录服务器启动信息
        Logger::log("Server starting on port: " + std::to_string(port));
        Logger::log("Document root: " + doc_root);
        Logger::log("Thread pool size: " + std::to_string(thread_pool_size));

        // 创建并启动服务器
        Server server(port, doc_root, thread_pool_size);
        server.start();
    } catch (const std::exception& ex) {
        // 捕获并记录异常
        Logger::log(std::string("Error: ") + ex.what());
        return 1;
    }
    return 0;
}
