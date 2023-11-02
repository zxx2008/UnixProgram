#ifndef _CLIENT_SOCKET_HANDLER_H_
#define _CLIENT_SOCKET_HANDLER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include "Defines.cpp"

//处理客户端请求的类
class ClientSocketHandler {
    private:
        char _buff[1024];    //缓冲区
        int _buffLength = 0;    //读取到的长度
        int _currentPoint = 0; //当前读到的位置
        int _clientSocket;  //客户端套接字
        
        bool ReadSocket();

    public:
        //构造函数
        ClientSocketHandler(int clientSocket);
        //按字符从buff中读取
        char GetNextByte();
        //析构函数
        ~ClientSocketHandler();
        bool WriteSocket(const char* buff, int length);
};

#endif 