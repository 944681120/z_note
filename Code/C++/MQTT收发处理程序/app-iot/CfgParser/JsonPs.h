#ifndef JSONPS_H
#define JSONPS_H

#pragma once
#include <vector>
#include <string>
#include "yyjson/yyjson.h"
#include "AbsCfgParser.h"

using namespace std;

namespace LLZ {   // 命名空间

class JsonPs : public AbsCfgParser
{
public:
    JsonPs() {};
    JsonPs(const char* filePath);
    JsonPs(const char* jsonStr, int strLen);
    ~JsonPs();

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
    int cfg_obj_iter(const char* keyPath, string& obj);
    int cfg_arr_iter(const char *keyPath, list<string>* obj = nullptr); 
    string cfg_print_str();
    int cfg_save_file(const char* filePath);

private:
    int cfg_write();
    int cfg_flush();
    void cfg_flg_init();

private:
    struct JsonPs_s
    {
        enum WorkMode_E { DEFAULT, FILEMODE, STRMODE }workMode = WorkMode_E::DEFAULT;
        string filePath;    // FILEMODE使用
        string jsonStr;     // STRMODE使用
        yyjson_doc *doc;
        yyjson_mut_doc* wdoc;
        yyjson_read_err rerr;
        yyjson_read_flag rflg;
        yyjson_write_err werr;
        yyjson_write_flag wflg;
    }*m = new JsonPs_s();
};

}   // 命名空间

#endif