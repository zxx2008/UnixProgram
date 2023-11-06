#ifndef _TOOL_H_
#define _TOOL_H_

#include <string>
#include "ClientSocketHandler.h"
//向客户端套接字发送文件工具
class Tools {
    public:
    static bool SendFile(std::string path, ClientSocketHandler* clientSocketHandler);
};

#endif