#include "FileScanner.h"
#include <string.h>
#include <algorithm>

bool direntCompare(const dirent obj1, const dirent obj2) {
    std::string str1(obj1.d_name);
    std::string str2(obj2.d_name);
    return str1 < str2;
}

FileScanner::FileScanner(std::string directoryPath) {
    _rootPath = directoryPath;
    Scan();
}

bool FileScanner::Scan() {
    DIR* dir;
    dirent* ptr;
    //打开目录，返回指向目录流的指针
    dir = opendir(_rootPath.c_str());
    if (dir == NULL) {
        PrintLine("打开目录失败");
        return false;
    }
    _fileAttrMap.clear();
    _fileAttrs.clear();
    //获取目录中的每一项
    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_type != DT_DIR && ptr->d_type != DT_REG) {
            continue;
        }
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {
            continue;
        }
        _fileAttrs.push_back(*ptr);
        _fileAttrMap.insert(std::pair<std::string, dirent>(ptr->d_name, *ptr));
    }
    SortFileAttr();
    return true;
}

void FileScanner::SortFileAttr() {
    //定义两个数组，分别存储目录和文件，先显示目录文件，再显示普通文件
    std::vector<dirent> dirs, files;
    for (int i = 0; i < _fileAttrs.size(); ++i) {
        if (_fileAttrs[i].d_type == DT_DIR) {
            dirs.push_back(_fileAttrs[i]);
        }
        if (_fileAttrs[i].d_type == DT_REG) {
            files.push_back(_fileAttrs[i]);
        }
    }
    //通过键的字典顺序进行排序
    //std::sort(dirs.begin(), dirs.end(), direntCompare);
    //std::sort(files.begin(), files.end(), direntCompare);
    //重新存入_fileAttrs
    _fileAttrs.clear();
    _fileAttrs.insert(_fileAttrs.end(), dirs.begin(), dirs.end());
    _fileAttrs.insert(_fileAttrs.end(), files.begin(), files.end());

}


dirent* FileScanner::GetDirentByIndex(int index) {
    if (index < 0 || index >= _fileAttrs.size()) {
        PrintLine("索引越界 index [%d]", index);
        return NULL;
    }
    return &_fileAttrs[index];
}

std::string FileScanner::GetFileNamByIndex(int index) {
    dirent* objPtr = GetDirentByIndex(index);
    if (objPtr == NULL) {
        return "";
    }
    return objPtr->d_name;
}

unsigned char FileScanner::GetFileTypeByIndex(int index) {
    dirent* objPtr = GetDirentByIndex(index);
    if (objPtr == NULL) {
        return 0;
    }
    return objPtr->d_type;
}

unsigned char FileScanner::GetFileTypeByName(std::string name) {
    //文件不存在
    if (_fileAttrMap.find(name.c_str()) == _fileAttrMap.end()) {
        PrintLine("文件不存在");
        return 0;
    }
    return _fileAttrMap[name.c_str()].d_type;
}

int FileScanner::GetLength() {
    return _fileAttrs.size();
}