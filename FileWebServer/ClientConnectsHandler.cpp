#include "ClientConnectsHandler.h"
#include <pthread.h>

bool ClientConnectsHandler::AddClientConnect(int clientSocket, sockaddr_in clientAddr) {
    pthread_t id;
    //在堆上申请内存空间，这样函数结束内存不会被回收
    ThreadData *threadData = new ThreadData(clientSocket, clientAddr);
    //创建新线程去响应客户端
    int ret = pthread_create(&id, NULL, Run, (void *)threadData);
    if (ret != 0) {
        PrintLine("线程创建失败 [%d]", ret);
        return false;
    }
    return true;
}

void* ClientConnectsHandler::Run(void* data) {
    // 读取客户端请求的对象
    ThreadData* threadData = (ThreadData *)data;
    ClientSocketHandler clientSocketHandler(threadData->ClientSocket);
    // 用于解析客户端请求的对象
    while (1)
    {
        PrintLine("解析客户端请求");
        ClientRequestAnalyzer clientRequestAnalyzer(&clientSocketHandler);
        //判断套接字关闭的情况
        if (clientRequestAnalyzer.AnalyzeSuccessed() == false) {
            PrintLine("退出客户端请求处理方法");
            break;
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
    delete threadData;
    return NULL;
}
