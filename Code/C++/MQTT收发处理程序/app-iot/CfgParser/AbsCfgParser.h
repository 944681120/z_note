#ifndef ABSCFGPARSER_H
#define ABSCFGPARSER_H

#pragma once
#include <list>
#include <vector>
#include <string>

using namespace std;

namespace LLZ {    // 命名空间

class AbsCfgParser
{
public:
    AbsCfgParser(){};
    virtual ~AbsCfgParser(){};

    int open(const char* filePathOrStr, int steLen = 0) { return -1; }
    virtual bool get_init() { return false; }
    virtual void *cfg_get_val(const char *keyPath) { return nullptr; }
    virtual int cfg_add_arr(const char *keyPath) { return -1; }
    virtual int cfg_get_int(const char *keyPath, int &val) { return -1; }
    virtual int cfg_get_str(const char *keyPath, string &val) { return -1; }
    virtual int cfg_get_bool(const char *keyPath, bool &val) { return -1; }
    virtual int cfg_get_double(const char *keyPath, double &val) { return -1; }
    virtual int cfg_set_int(const char *keyPath, const int &val) { return -1; }
    virtual int cfg_set_str(const char *keyPath, const string &val) { return -1; }
    virtual int cfg_set_bool(const char *keyPath, const bool &val) { return -1; }
    virtual int cfg_set_double(const char *keyPath, const double &val) { return -1; }
    virtual int cfg_add_int(const char *keyPath, const int &val) { return -1; }
    virtual int cfg_add_str(const char *keyPath, const string &val) { return -1; }
    virtual int cfg_add_bool(const char *keyPath, const bool &val) { return -1; }
    virtual int cfg_add_double(const char *keyPath, const double &val) { return -1; }
    virtual int cfg_obj_iter(const char *keyPath, string &obj) { return -1; }       // 获取一个子对象
    virtual int cfg_arr_iter(const char *keyPath, list<string>* obj = nullptr) { return -1; } // 获取一个数组里的条目
    virtual string cfg_print_str() { return string(); }
    virtual int cfg_save_file(const char* filePath) { return -1; }
};  

}   // 命名空间


#endif