#include "ClientRequestAnalyzer.h"
#include <algorithm>

ClientRequestAnalyzer::ClientRequestAnalyzer(ClientSocketHandler* clientSocketHandler) {
    std::vector<std::string> lines;     //将客户端的每一行存在一个字符串中，并将每一个字符串存储到字符串向量中
    std::string tempStr = "";
    while (1) {
        char c;
        //判断套接字关闭的情况
        if (clientSocketHandler->GetNextByte(&c) == false) {
            PrintLine("套接字关闭");
            _analyzeSuccessed = false;
            return;
        }
        printf("%c", c);
        tempStr += c;
        if(tempStr.length() >= 2 && tempStr[tempStr.length()-1] == '\n' && tempStr[tempStr.length() - 2] == '\r') {
            if (tempStr.length() == 2) {
                break;
            }
            tempStr.erase(tempStr.length() - 2);
            lines.push_back(tempStr);
            tempStr.clear();
        }
    }
    AnalyzeFirstLine(lines[0]);
    for (int i = 1; i < lines.size(); ++i) {
        AnalyzeDataLine(lines[i]);
    }
    _analyzeSuccessed = true;
}

void ClientRequestAnalyzer::AnalyzeFirstLine(std::string line) {
    std::vector<std::string> strs;
    std::string tempStr = "";
    //以空格为分隔符分隔字符串
    for (int i = 0; i < line.length(); ++i) {
        if (line[i] == ' ' && tempStr.length() > 0) {
            strs.push_back(tempStr);
            tempStr.clear();
            continue;
        }
        tempStr += line[i];
    }
    if (tempStr.length() > 0) {
        strs.push_back(tempStr);
    }
    //第一行的三个字符串
    _requestName = strs[0];
    _url = strs[1];
    _httpVersion = strs[2];
}

void ClientRequestAnalyzer::AnalyzeDataLine(std::string line) {
    int i = 0;
    RequestKeyData keyData;
    //以冒号为分隔符
    for (i = 0; i < line.length(); ++i) {
        if (line[i] == ':') {
            keyData.Key = line.substr(0, i);
            i++;
            while (line[i] == ' ') {
                ++i;
            }
        }
        keyData.Data = line.substr(i);
        std::transform(keyData.Key.begin(), keyData.Key.end(), keyData.Key.begin(), ::tolower);
        _keyDataMap.insert(std::pair<std::string, RequestKeyData>(keyData.Key, keyData));
    }
}

std::string ClientRequestAnalyzer::GetRequestName() {
    return _requestName;
}

std::string ClientRequestAnalyzer::GetUrl() {
    return _url;
}

std::string ClientRequestAnalyzer::GetHttpVersion() {
    return _httpVersion;
}

std::string ClientRequestAnalyzer::GetKeyDataByKey(std::string key) {
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if (_keyDataMap.find(key) == _keyDataMap.end()) {
        return "";
    }
    return _keyDataMap[key].Data;
}

bool ClientRequestAnalyzer::AnalyzeSuccessed() {
    return _analyzeSuccessed;
}