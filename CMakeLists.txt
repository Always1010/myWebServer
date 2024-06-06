cmake_minimum_required(VERSION 3.10)
project(my_web_server)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 添加 include 目录到头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# 设置源文件,不包含main.cpp
set(SOURCES
    src/server/Server.cpp
    src/http/HttpRequest.cpp
    src/http/HttpResponse.cpp
    src/utils/Logger.cpp
    src/utils/Config.cpp
    src/handlers/RequestHandler.cpp
)

# 生成可执行文件
add_executable(my_web_server src/main.cpp ${SOURCES})


# 启用测试
enable_testing()

# 查找已安装的 Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# 添加测试源文件
set(TEST_SOURCES
    tests/test_server.cpp
    tests/test_http_request.cpp
    # 其他测试文件
)

# 生成测试可执行文件
add_executable(runTests ${TEST_SOURCES} ${SOURCES})

# 链接测试库
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

add_test(NAME runTests COMMAND runTests)