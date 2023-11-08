#include "ClientSocketHandler.h"
#include <unistd.h>


bool ClientSocketHandler::ReadSocket() {
    //读取客户端的请求到buff中
    //PrintLine("recv start");
    _buffLength = recv(_clientSocket, _buff, sizeof(_buff), MSG_NOSIGNAL);
    //PrintLine("recv finish");
    if (_buffLength < 0) {
        PrintLine("读取客户端内容失败");
        return false;
    }
    return true;
}

bool ClientSocketHandler::WriteSocket(const char* buff, int length) {
    int ret = send(_clientSocket, buff, length, MSG_NOSIGNAL);
    if (ret < 0) {
        return false;
    }
    return true;
}

//构造函数
ClientSocketHandler::ClientSocketHandler(int clientSocket) {
    _clientSocket = clientSocket;
    ReadSocket();
    _currentPoint = 0;
}

bool ClientSocketHandler::GetNextByte(char* c)  {
    //如果当前缓冲区读完，再次ReadSocket()
    if(_currentPoint == _buffLength) {
        //判断套接字关闭的情况
        if (ReadSocket() == false) {
            PrintLine("套接字已关闭");
            return false;
        }
         _currentPoint = 0;
    }
    //返回当前字符，并且currentPoint++
     *c = _buff[_currentPoint++];
    return true;
}

ClientSocketHandler::~ClientSocketHandler() {
    close(_clientSocket);
}