#ifndef _CLIENT_CONNECTS_HANDLER_H_
#define _CLIENT_CONNECTS_HANDLER_H_

#include "ClientSocketHandler.h"
#include "ClientRequestAnalyzer.h"
#include "Defines.cpp"
#include "Tools.h"
#include "FileHandler.h"
#include <netinet/in.h>
#include "UrlConverter.cpp"
#include "FileScanner.h"

class ClientConnectsHandler {
    private:
    struct ThreadData {
        public:
        int ClientSocket;
        sockaddr_in ClientAddr;
        ThreadData(int socket, sockaddr_in addr) {
            ClientSocket = socket;
            ClientAddr = addr;
        }
    };
    public:
    //添加客户端连接
    bool AddClientConnect(int clientSocket, sockaddr_in clientAddr);
    //线程执行函数
    static void* Run(void* data);
};

#endif