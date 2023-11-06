#include "FileHandler.h"

FileHandler::FileHandler(std::string path, std::string mode) {
    _filePath = path;
    //判断文件是否存在
    if (IsFileExist() == false) {
        PrintLine("文件不存在 [%s]", _filePath.c_str());
        return;
    }
    //打开文件
    _file = fopen(_filePath.c_str(), mode.c_str());
    if (_file == NULL) {
        PrintLine("文件打开失败 [%s]", _filePath.c_str());
        return;
    }
}


bool FileHandler::IsDirectoryFile() {
    struct stat st;
    stat(_filePath.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

bool FileHandler::IsOrdinaryFile() {
    struct stat st;
    stat(_filePath.c_str(), &st);
    return S_ISREG(st.st_mode);
}

bool FileHandler::IsFileExist() {
    return (access(_filePath.c_str(), F_OK) == 0 &&  (IsOrdinaryFile() || IsDirectoryFile()));
}

long FileHandler::GetFileSize() {
    if (_file == NULL) {
        PrintLine("文件指针为空");
        return -1;
    }
    //将指针指向文件末尾
    fseek(_file, 0L, SEEK_END);
    // The  ftell()  function  obtains  the current value of the file position indicator for the stream pointed to by
      // stream.
    long ret = ftell(_file);
    fseek(_file, _currentFp, SEEK_SET);
    return ret;
}

int FileHandler::Read(char* buff, int length) {
    int ret = fread(buff, sizeof(char), length, _file);
    _currentFp = ftell(_file);
    return ret;
}