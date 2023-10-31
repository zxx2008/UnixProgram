#include "Server.h"

bool Server::initServer(uint16_t* port) {
    //设置端口
    _serverport = *port;
    //创建套件字，PF_INET: ipv4协议，SOCK_STREAM: TCP协议
    _serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1) {
        std::cout << "套接字创建失败" << std::endl;
        return false;
    }
    //设置套剪子端口可复用性
    int opt = 1;
    int ret = setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (ret == -1) {
        std::cout << "设置端口可复用性失败" << std::endl;
        return false;
     }
    //配置网络服务器的网络地址的属性
    sockaddr_in serverAdder;
    serverAdder.sin_family = AF_INET;   //ipv4
    serverAdder.sin_port = htons(_serverport);     //主机字节序转变为网络字节序
    serverAdder.sin_addr.s_addr = INADDR_ANY;
    //绑定套件字和网络地址属性
    ret = bind(_serverSocket, (sockaddr*)&serverAdder, sizeof(serverAdder));
    if (ret == -1) {
        std::cout << "套件字绑定失败" << std::endl;
        return false;
    }
    //如果传入的端口为0，随机分配一个端口
    if (0 == *port) {
        int length = sizeof(serverAdder);
        ret = getsockname(_serverSocket, (sockaddr*)&serverAdder, (socklen_t*)&length);
        if (ret < 0) {
            std::cout << "随机端口分配失败" << std::endl;
            return false;
        }
        _serverport = serverAdder.sin_port;
        *port = _serverport;
    }
    return true;
}

uint16_t Server::GetServerPort() {
    return _serverport;
}

int Server::GetServerSocket() {
    return _serverSocket;
}

bool Server::Listen() {
    return listen(_serverSocket, queueLength) == 0;
}

bool Server::Accept(ClientData* clientData) {
    int length = sizeof(clientData->ClientAddr);
    clientData->ClientSock = accept(_serverSocket, (sockaddr*)&(clientData->ClientAddr), (socklen_t*)length);
    return clientData->ClientSock != -1;
}