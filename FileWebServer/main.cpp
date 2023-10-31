#include "Server.h"

int main() {
    uint16_t port = 80;
    //初始化服务器
    Server server;
    if (server.initServer(&port) == false) {
        std::cout << "初始化服务器失败" << std::endl;
        return 0;
    }
    std::cout << "初始化服务器成功" << std::endl;
    return 0;
}