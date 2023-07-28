#ifndef _MAIN_H_
#define _MAIN_H_

#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <mutex>
#include <thread>
#include "zcom.h"
#include "IniPs.h"
#include "lua.hpp"
#include "JsonPs.h"
#include "Daemon.h"
#include "MqttClt.h"
#include "IotProject.h"
#include "Serialport.h"

static const char *str_opt = "vVh?";
static const char *str_help = "Usage: %s [-v,V] [-?] [-h]";
#define APP_VERSION "V1.0.0"

struct main_t
{
    lua_State* L;
    string str;
    static long int offset_L;
};

template <typename T>
int appHelp(int argc, char *argv[])
{
    int ret = getopt(argc, argv, str_opt);

    while (-1 != ret)
    {
        switch (ret)
        {
        case 'v':
        case 'V':
            printf("%s\n", APP_VERSION);
            exit(0);

        case 'h':
        case '?':
            printf(str_help, argv[0]);
            exit(0);

        default:
            break;
        }
        ret = getopt(argc, argv, str_opt);
    }
    return 0;
}

#endif  // _MAIN_H_