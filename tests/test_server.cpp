#include "server/Server.h"
#include <gtest/gtest.h>

// TEST(ServerTest, Start) {
//     Server server;
//     server.start();
//     // // 添加断言
//     // ASSERT_NO_THROW(server.start());
//     std::cout<<"this is a test!"<<std::endl;
// }

// Google Test 提供的 main 函数
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
