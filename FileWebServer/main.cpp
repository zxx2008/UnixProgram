#include "Server.h"

int ReadSocket(int clientSock, char *buff, int length) {
    return recv(clientSock, buff, length, MSG_NOSIGNAL);
}

int WriteSocket(int clientSock, char *buff, int length) {
    return send(clientSock, buff, length, MSG_NOSIGNAL);
}

void HandleClientRequest(Server::ClientData *clientData) {
    char buff[10240];
    int length = ReadSocket(clientData->ClientSock, buff, 10240);
    printf("%s\n",buff);
    char header[] = "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\nContent-type:text/html\r\n\r\n";
    char htmlText[] = "<html><a>hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world </a></html>";
    int ret = WriteSocket(clientData->ClientSock, header, sizeof(header));
    printf("sent %d\n", ret);
    ret = WriteSocket(clientData->ClientSock, htmlText, sizeof(htmlText));
    printf("sent %d\n", ret);
     
}

 

int main() {
    uint16_t port = 80;
    //初始化服务器
    Server server;
    if (server.initServer(&port) == false) {
        std::cout << "初始化服务器失败" << std::endl;
        return 0;
    }
    std::cout << "初始化服务器成功" << std::endl;
    //return 0;
    if (server.Listen(10) == false) {
        std::cout << "服务端监听失败" << std::endl;
        return 0;
    }
    while (1) {
        Server::ClientData clientData;
        if (server.Accept(&clientData) == false) {
            std::cout << "创建客户端套接字失败" << std::endl;
            continue;
        }
        unsigned char* ip = (unsigned char*)&(clientData.ClientAddr.sin_addr.s_addr);
        std::cout << "创建客户端套接字成功" << ip[0] << ", " << ip[1] << ", " << ip[2] << ", " << ip[3] << std::endl;
        HandleClientRequest(&clientData);
    }
    return 0;
}