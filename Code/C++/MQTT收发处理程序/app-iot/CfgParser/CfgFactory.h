#ifndef CFGFACTORY_H
#define CFGFACTORY_H

#pragma once
#include <memory>
#include "AbsCfgParser.h"

namespace LLZ {   // 命名空间

class CfgFactory
{
public:
    /// @brief 解析配置文件简单工厂函数，创建不同的解析配置文件对象
    /// @param type         ：创建的解析文件类型 yyjson:json解析, simpleini:ini解析
    /// @param filepathOrStr：打开的cfg文件路径或者cfg字符串
    /// @param strLen       ：打开为cfg字符串时，该参数需要填入字符串的长度值
    static AbsCfgParser* newObj(const std::string &type, const char* filepathOrStr, int strLen = -1);
    static bool delObj(AbsCfgParser* obj);
};

}   // 命名空间

#endif