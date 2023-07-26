#ifndef INIPS_H
#define INIPS_H

#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "simpleini/SimpleIni.h"
#include "AbsCfgParser.h"

#include "yyjson/yyjson.h"

using namespace std;

namespace LLZ {   // 命名空间

class IniPs : public AbsCfgParser
{
public:
    IniPs() {};
    IniPs(const char* filePath);
    IniPs(const char* IniStr, int strLen);
    ~IniPs();
    
    bool get_init();
    int open(const char* filePathOrStr, int steLen = 0);
    void* cfg_get_val(const char* keyPath); 
    int cfg_add_arr(const char* keyPath);
    int cfg_get_int(const char* keyPath, int& val);
    int cfg_get_str(const char* keyPath, string& val);
    int cfg_get_bool(const char* keyPath, bool& val);
    int cfg_get_double(const char* keyPath, double& val);
    int cfg_set_int(const char* keyPath, const int& val);
    int cfg_set_str(const char* keyPath, const string& val);
    int cfg_set_bool(const char* keyPath, const bool& val);
    int cfg_set_double(const char* keyPath, const double& val);
    int cfg_add_int(const char* keyPath, const int& val);
    int cfg_add_str(const char* keyPath, const string& val);
    int cfg_add_bool(const char* keyPath, const bool& val);
    int cfg_add_double(const char* keyPath, const double& val);
    int cfg_obj_iter(const char* keyPath, string &obj);
    int cfg_arr_iter(const char *keyPath, list<string>* obj = nullptr); // 获取一个数组下的全部值,obj为null时则只返回数量
    string cfg_print_str();
    int cfg_save_file(const char* filePath);

private:
    int cfg_write();
    int cfg_flush();
    void cfg_flg_init();
    int pick_keyPath(const char* keyPath, vector<string>& oVecStr);

private:
    struct IniPs_s
    {
        enum WorkMode_E { DEFAULT, FILEMODE, STRMODE }workMode = WorkMode_E::DEFAULT;
        string filePath;    // FILEMODE使用
        string IniStr;     // STRMODE使用
        SI_Error rc;
        CSimpleIniA ini;
        bool initFlg = false;
        vector<string> keyPathVecStr;

        yyjson_doc *doc;
        yyjson_mut_doc* wdoc;
        yyjson_read_err rerr;
        yyjson_read_flag rflg;
        yyjson_write_err werr;
        yyjson_write_flag wflg;
        string jsonStr;     // STRMODE使用
    }*m = new IniPs_s();
};

}   // 命名空间

#endif