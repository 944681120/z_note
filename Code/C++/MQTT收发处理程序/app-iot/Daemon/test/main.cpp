#include "zcom.h"
#include "Daemon.h"
#include "JsonPs.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    JsonPs yjsonp("./deamon.json");
    /* 获取全部的key值，再获取全部的value值 */
    vector<string> keys, vals;
    yjsonp.cfg_obj_iter("/app_list/default_open", keys);    // key
    for (int i = 0; i < keys.size(); i++)
    {
        string str("/app_list/default_open/" + keys[i]);     // path
        yjsonp.cfg_get_str(str.c_str(), str);  // val
        printf("%s : %s\n", keys[i].c_str(), str.c_str());
        vals.push_back(str);
    }

    /* 创建守护进程对象 */
    Daemon daem;
    for (size_t i = 0; i < keys.size(); i++)
    {
        daem.daemByProgram(keys[i].c_str(), vals[i].c_str(), true);
    }

    while(1);
    return 0;
}
