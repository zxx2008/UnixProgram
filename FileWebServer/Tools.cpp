#include "Tools.h"

bool Tools::SendFile(std::string path, ClientSocketHandler* clientSocketHandler) {
    //打开文件
    FILE* file = fopen(path.c_str(), "r");
    if (file == NULL) {
        PrintLine("打开文件失败");
        return false;
    }
    char buff[4096];
    int buffLength = 0;
    while ((buffLength = fread(buff, sizeof(char), sizeof(buff), file)) > 0) {
        if (clientSocketHandler->WriteSocket(buff, buffLength) == false) {
            PrintLine("文件发送失败");
            fclose(file);
            return false;
        }
    }
    fclose(file); 
    return true;
}