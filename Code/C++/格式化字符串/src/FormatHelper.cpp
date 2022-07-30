#include "FormatHelper.h"
#include <stdarg.h>
#include <regex>
#include <iostream>

FormatHelper::FormatHelper()
{

}

FormatHelper::~FormatHelper()
{

}

/*
    描述：检测是否为hexstr
    参数：
    返回：
*/
bool FormatHelper::CheckHexstr(const string& data)
{
    // 当字符串长度小于1或是奇数的时候返回false
    if(data.length() < 1 || data.length() % 2 == 1)
    {
        return false;
    }
    // 返回正则判断结果
    regex reg("^[0-9a-fA-F]+$");
    return regex_match(data, reg);
}

/*
    描述：字符串处理，格式化转字符串
    参数：
    返回：格式化的字符串
*/
vector<uint8_t> FormatHelper::HexstrToBytes(const string& hex)    //abcd
{
    if( CheckHexstr(hex) == false ) 
    {
        return vector<uint8_t>();
    }

    vector<uint8_t> bytes;
    for (unsigned long i = 0; i < hex.length(); i += 2)
    {
        bytes.push_back(stoi(hex.substr(i, 2), nullptr, 16));
    }
    return bytes;
}

/*
    描述：字符串处理，格式化转字符串
    参数：
    返回：格式化的字符串
*/
string FormatHelper::Format(const char* fmt, ...)
{
    char c[1024] = {0};
    va_list args;
    va_start(args, fmt);
    vsnprintf(c, 1024, fmt, args);
    va_end(args);

    return string(c);
}