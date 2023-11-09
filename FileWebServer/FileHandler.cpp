#include "FileHandler.h"

FileHandler::FileHandler(std::string path, std::string mode)
{
    _filePath = path;
    // 判断文件是否存在
    if (IsFileExist() == false)
    {
        PrintLine("文件不存在 [%s]", _filePath.c_str());
        return;
    }
    // 打开文件
    _file = fopen(_filePath.c_str(), mode.c_str());
    if (_file == NULL)
    {
        PrintLine("文件打开失败 [%s]", _filePath.c_str());
        return;
    }
}

bool FileHandler::IsDirectoryFile()
{
    return IsDirectoryFile(_filePath);
}

bool FileHandler::IsOrdinaryFile()
{
    return IsOrdinaryFile(_filePath);
}

bool FileHandler::IsFileExist()
{
    return IsFileExist(_filePath);
}

long FileHandler::GetFileSize()
{
    return GetFileSize(_filePath);
}

int FileHandler::Read(char *buff, int length)
{
    int ret = fread(buff, sizeof(char), length, _file);
    _currentFp = ftell(_file);
    return ret;
}

long FileHandler::GetFileSize(std::string path)
{
    FILE *file = fopen(path.c_str(), "r");
    if (file == NULL)
    {
        PrintLine("文件指针为空 %s", path.c_str());
        return -1;
    }
    fseek(file, 0L, SEEK_END);
    long ret = ftell(file);
    fclose(file);
    return ret;
}

bool FileHandler::IsDirectoryFile(std::string path)
{
    struct stat st;
    stat(path.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

bool FileHandler::IsOrdinaryFile(std::string path)
{
    struct stat st;
    stat(path.c_str(), &st);
    return S_ISREG(st.st_mode);
}

bool FileHandler::IsFileExist(std::string path) {
    return access(path.c_str(), F_OK) == 0 && (IsOrdinaryFile(path) || IsDirectoryFile(path));
}