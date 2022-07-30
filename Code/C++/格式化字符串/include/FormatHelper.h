#ifndef FORMATHELPER_H
#define FORMATHELPER_H

#include <string>
#include <vector>

#pragma once
using namespace std;

class FormatHelper
{
public:
    FormatHelper();
    ~FormatHelper();

    static bool CheckHexstr(const string& data);
    static vector<uint8_t> HexstrToBytes(const string& hex);
    string Format(const char* fmt, ...);

private:

};

#endif