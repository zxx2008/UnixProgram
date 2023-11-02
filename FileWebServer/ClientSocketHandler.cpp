#include "ClientSocketHandler.h"
#include <unistd.h>


int ClientSocketHandler::ReadSocket() {
    //读取客户端的请求到buff中
    //PrintLine("recv start");
    _buffLength = recv(_clientSocket, _buff, sizeof(_buff), MSG_NOSIGNAL);
    //PrintLine("recv finish");
    if (_buffLength < 0) {
        return false;
    }
        return true;
}

int ClientSocketHandler::WriteSocket(char* buff, int length) {
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

char ClientSocketHandler::GetNextByte()  {
    //如果当前缓冲区读完，再次ReadSocket()
    if(_currentPoint == _buffLength) {
        ReadSocket();
         _currentPoint = 0;
    }
    //返回当前字符，并且currentPoint++
    return _buff[_currentPoint++];
}

ClientSocketHandler::~ClientSocketHandler() {
    close(_clientSocket);
}