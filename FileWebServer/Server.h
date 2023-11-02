#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

class Server {
    private:
        uint16_t _serverport;
        int _serverSocket;
    public:
        struct ClientData {
            public:
                sockaddr_in ClientAddr;
                int ClientSock;
        };
    public:
        //初始化服务器
        bool initServer(uint16_t* port);
        //获取_serverport
        uint16_t GetServerPort();
        int GetServerSocket();
        bool Listen(int queueLength);
        bool Accept(ClientData* clientData);
};

#endif