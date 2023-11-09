#ifndef _FILE_HANDLER_H_
#define _FILE_HANDLER_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Defines.cpp"

//用于获取文件大小，在发送相应头时可以添加Content-Length
class FileHandler {
    private:
    FILE* _file = NULL;
    std::string _filePath;
    long _currentFp = 0;
    public:
    FileHandler(std::string path, std::string mode = "r");
    //判断文件是否存在
    bool IsFileExist();
    //判断是否为目录文件
    bool IsDirectoryFile();
    //判断是否为普通文件
    bool IsOrdinaryFile();
    //获取文件大小
    long GetFileSize();
    
    int Read(char* buff, int length);

    static long GetFileSize(std::string path);
    static bool IsDirectoryFile(std::string path);
    static bool IsOrdinaryFile(std::string path);
    static bool IsFileExist(std::string path);
};

#endif