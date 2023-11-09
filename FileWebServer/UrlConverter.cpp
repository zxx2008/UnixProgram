#ifndef URL_CONVERTER
#define URL_CONVERTER

#include <string>
#include "Defines.cpp"

class UrlConverter
{
public:
    static unsigned char HexCharToInt8(char c)
    {
        if (c >= 'a' && c <= 'f')
        {
            return c - 'a' + 10;
        }
        if (c >= 'A' && c <= 'F')
        {
            return c - 'A' + 10;
        }
        if (c >= '0' && c <= '9')
        {
            return c - '0';
        }
        PrintLine("非法字符 [%c]", c);
        return -1;
    }

    static std::string Int8ToHexChar(unsigned char c)
    {
        char list[] = "0123456789ABCDEF";
        std::string ret = "";
        ret += list[c / 16];
        ret += list[c % 16];
        return ret;
    }

    static std::string ConvertFromUrl(std::string url)
    {
        std::string ret = "";
        for(int i = 0;i<url.length();i++)
        {
            if(url[i] == '%')
            {
                char c = HexCharToInt8(url[i+1]);
                c=c<<4;
                c += HexCharToInt8(url[i+2]);
                ret += c;
                i+=2;
            }
            else
            {
                ret += url[i];
            }
        }
        return ret;
    }

    static std::string ConvertToUrl(std::string str)
    {
        std::string ret = "";
        for(int i=0;i<str.length();i++)
        {
            if(std::isalnum(str[i]) || str[i] == '.' || str[i] == '/')
            {
                ret += str[i];
            }
            else
            {
                ret += "%";
                ret += Int8ToHexChar(str[i]);
            }
        }
        return ret;
    }
};

#endif