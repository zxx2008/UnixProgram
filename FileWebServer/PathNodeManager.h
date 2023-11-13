#ifndef _PATH_NODE_MANAGER_H_
#define _PATH_NODE_MANAGER_H_

#include <string>
#include <vector>
#include "Defines.cpp"

class PathNodeManager {
    private:
    std::vector<std::string> _nodeNames;
    public:
    PathNodeManager(std::string urlPath);
    /// @brief 通过索引获得文件名
    /// @param index 索引
    /// @return 文件名
    std::string GetNameByIndex(int index);

    /// @brief GetNameByIndex的URL版本
    /// @param index 索引
    /// @return 文件名的Url版本
    std::string GetNameByIndex_UrlMode(int index);

    /// @brief 通过索引获取超链接路径
    /// @param index 索引
    /// @return 路径
    std::string GetPath(int index);

    /// @brief GetPath的Url版本
    /// @param index 索引
    /// @return 路径的Url版本
    std::string GetPath_UrlMode(int index);

    /// @brief 获取_nodeNames的长度
    /// @return _nodeNames的长度
    int GetLength();
};

#endif