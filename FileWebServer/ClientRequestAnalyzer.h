#ifndef _CLIENT_REQUEST_ANALYZER_H_
#define _CLIENT_REQUEST_ANALYZER_H_

#include <string.h>
#include <map>
#include <vector>
#include "ClientSocketHandler.h"
#include "Defines.cpp"

//分析并处理客户端的请求
class ClientRequestAnalyzer {
    public:
        struct RequestKeyData {
            public:
                std::string Key, Data;
        };
    private:
        std::string _requestName, _url, _httpVersion;
        std::map<std::string, RequestKeyData> _keyDataMap;

    public:
        //构造函数
        ClientRequestAnalyzer(ClientSocketHandler* clientSocketHandler);

        //分析请求第一行
        void AnalyzeFirstLine(std::string line);

        //分析数据行
        void AnalyzeDataLine(std::string line);

        //获取RequestName
        std::string GetRequestName();

        //获取URL
        std::string GetUrl();

        //获取HTTP版本
        std::string GetHttpVersion();

        //通过Key获取值
        std::string GetKeyDataByKey(std::string key);

};



#endif