#include "Server.h"
#include <unistd.h>
#include "ClientConnectsHandler.h"

/*int ReadSocket(int clientSock, char *buff, int length) {
    return recv(clientSock, buff, length, MSG_NOSIGNAL);
}

int WriteSocket(int clientSock, char *buff, int length) {
    return send(clientSock, buff, length, MSG_NOSIGNAL);
}*/

// 处理客户端请求函数
void HandleClientRequest(Server::ClientData *clientData)
{
    // 读取客户端请求的对象
    ClientSocketHandler clientSocketHandler(clientData->ClientSock);
    // 用于解析客户端请求的对象
    while (1)
    {
        PrintLine("解析客户端请求");
        ClientRequestAnalyzer clientRequestAnalyzer(&clientSocketHandler);
        //判断套接字关闭的情况
        if (clientRequestAnalyzer.AnalyzeSuccessed() == false) {
            PrintLine("退出客户端请求处理方法");
            return;
        }
        PrintLine("...");
        PrintLine("head name: [%s]", clientRequestAnalyzer.GetRequestName().c_str());
        PrintLine("url: [%s]", clientRequestAnalyzer.GetUrl().c_str());
        PrintLine("http version: [%s]", clientRequestAnalyzer.GetHttpVersion().c_str());
        PrintLine("Cache-control: [%s]", clientRequestAnalyzer.GetKeyDataByKey("Cache-Control").c_str());

        // web服务器根目录
        std::string rootPath = "WebRoot";
        // 根目录 + 请求目录
        std::string path = rootPath + clientRequestAnalyzer.GetUrl();

        // 初始化FileHandler对象，获取文件大小
        FileHandler fileHandler(path);

        // 响应头
        std::string header = "HTTP/1.1 200 OK\r\nServer:Httpd/1.1\r\n";
        header += "Content-Length:" + std::to_string(fileHandler.GetFileSize()) + "\r\n";
        header += "Content-type:image/jpeg\r\n\r\n";
        clientSocketHandler.WriteSocket(header.c_str(), header.length());

        // 发送文件
        Tools::SendFile(path, &clientSocketHandler);
    }
}

int main()
{
    uint16_t port = 80;
    // 初始化服务器
    Server server;
    if (server.initServer(&port) == false)
    {
        std::cout << "初始化服务器失败" << std::endl;
        return 0;
    }
    std::cout << "初始化服务器成功" << std::endl;
    // return 0;
    if (server.Listen(10) == false)
    {
        std::cout << "服务端监听失败" << std::endl;
        return 0;
    }
    ClientConnectsHandler clientConnectsHandler;

    while (1)
    {
        Server::ClientData clientData;
        if (server.Accept(&clientData) == false)
        {
            std::cout << "创建客户端套接字失败" << std::endl;
            continue;
        }
        unsigned char *ip = (unsigned char *)&(clientData.ClientAddr.sin_addr.s_addr);
        std::cout << "创建客户端套接字成功" << ip[0] << ", " << ip[1] << ", " << ip[2] << ", " << ip[3] << std::endl;
        clientConnectsHandler.AddClientConnect(clientData.ClientSock, clientData.ClientAddr);
        // close(clientData.ClientSock);
    }
    return 0;
}