#ifndef _FILE_SCANNER_H_
#define _FILE_SCANNER_H_

#include <string>
#include <vector>
#include <map>
#include <dirent.h>
#include "Defines.cpp"

class FileScanner {
    private:
    //根目录
    std::string _rootPath;
    //目录中的项
    std::vector<dirent> _fileAttrs;
    std::map<std::string, dirent> _fileAttrMap;
    /// @brief 通过索引获得dirent对象
    /// @param index 索引值
    /// @return dirent对象
    dirent* GetDirentByIndex(int index);
    public:
    FileScanner(std::string directoryPath);
    //扫描目录
    bool Scan();
    void SortFileAttr();

    /// @brief 通过索引获得文件名
    /// @param index 索引
    /// @return 返回文件名
    std::string GetFileNamByIndex(int index);

    /// @brief 通过索引获得文件类型
    /// @param index 索引
    /// @return 返回文件类型
    unsigned char GetFileTypeByIndex(int index);

    /// @brief 通过文件名获得文件类型
    /// @param name 文件名
    /// @return 返回文件类型
    unsigned char GetFileTypeByName(std::string name);

    /// @brief 返回目录项目数量
    /// @return 目录项目数量
    int GetLength();
};

/// @brief 自定义排序方式
/// @param obj1 
/// @param obj2 
/// @return 
bool direntCompare(const dirent obj1, const dirent obj2);

#endif

