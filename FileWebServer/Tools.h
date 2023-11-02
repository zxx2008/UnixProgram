#ifndef _TOOL_H_
#define _TOOL_H_

#include <string>
#include "ClientSocketHandler.h"
class Tools {
    public:
    static bool SendFile(std::string path, ClientSocketHandler* clientSocketHandler);
};

#endif