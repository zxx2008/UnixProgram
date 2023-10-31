#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Server {
    private:
        uint16_t _serverport;
        int _serverSocket;
        struct ClientData {
            public:
                sockaddr ClientAddr;
                int ClientSock;
        };
    public:
        //初始化服务器
        bool initServer(uint16_t* port);
        //获取_serverport
        uint16_t GetServerPort();
        int GetServerSocket();
        bool Listen(int queueLength);
        bool Accept();
};