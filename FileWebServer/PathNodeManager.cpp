#include "PathNodeManager.h"
#include "UrlConverter.cpp"

PathNodeManager::PathNodeManager(std::string urlPath)
{
    //PrintLine("%d", (int)urlPath.length());
    std::string name = "";
    for (int i = 0; i < urlPath.length(); ++i)
    {
        if (urlPath[i] == '/')
        {
            if (name.length() > 0)
            {
                _nodeNames.push_back(name);
                //PrintLine("添加%s", name.c_str());
                name.clear();
            }
        }
        else
        {
            name += urlPath[i];
        }
    }
    if (name.length() > 0)
    {
        _nodeNames.push_back(name);
        //PrintLine("添加%s", name.c_str());
    }
}

std::string PathNodeManager::GetNameByIndex_UrlMode(int index) {
    return UrlConverter::ConvertToUrl(GetNameByIndex(index));
}

std::string PathNodeManager::GetNameByIndex(int index) {
    if (index < 0 || index >= _nodeNames.size()) {
        PrintLine("index越界 index[%d], node Size [%d]", index, (int)_nodeNames.size());
        return "";
    }
    return _nodeNames[index];
}

std::string PathNodeManager::GetPath(int index) {
    std::string path = "/";
    if (index < 0 || index >= _nodeNames.size()) {
        PrintLine("index越界 index[%d], node Size [%d]", index, (int)_nodeNames.size());
        return path;
    }
    for (int i = 0; i <= index; ++i) {
        path += _nodeNames[i];
        path += "/";
    }
    return path;
}

std::string PathNodeManager::GetPath_UrlMode(int index) {
    std::string path = "/";
    if (index < 0 || index >= _nodeNames.size()) {
        PrintLine("index越界 index[%d], node Size [%d]", index, (int)_nodeNames.size());
        return path;
    }
    for (int i = 0; i <= index; ++i) {
        path += GetNameByIndex_UrlMode(index);
        path += "/";
    }
    return path;
}

int PathNodeManager::GetLength() {
    return _nodeNames.size();
}