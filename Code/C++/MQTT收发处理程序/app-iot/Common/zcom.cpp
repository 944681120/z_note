#include "zcom.h"
#include <stdio.h>
#include <stdlib.h>

namespace LLZ {    // 命名空间haed

int detect_process(const char* process_name)
{
    int n = -1;
    FILE *strm;
    char buf[128] = {0};
    sprintf(buf, "ps x | grep -E '%s .*|%s$' | grep -v grep", process_name, process_name);
    
    if ((strm = popen(buf, "r")) != NULL)
    {
        if (fgets(buf, sizeof(buf), strm) != NULL)
        {
            n = atoi(buf);
        }
    }
    else
    {
        return -1;
    }

    pclose(strm);
    return n;
}

int kill_process(const char *process_name)
{
    char cmd[128] = {0};
    int pid = detect_process(process_name);
    if ( pid >= 0 )
    {
        sprintf(cmd, "kill -9 %d", pid);
        system(cmd);
        sprintf(cmd, "killall %s", process_name);
        system(cmd);
    }
    
    return 0;
}

int creatThread(void *(*pFunc)(void *), void *arg)
{
    if ( pFunc == nullptr )
    {
        return -1;
    }
    
    // 创建并启动分离线程
    std::thread detachedThread(pFunc, arg);
    // 分离线程
    detachedThread.detach();
    return 0;
}

string hexToStr(const unsigned char* hexBuf, int bufLen)
{
    string str;
    for (int i = 0; i < bufLen; i++)
    {
        char str1 = (hexBuf[i] >> 4)    + '0';
        char str2 = (hexBuf[i] & 0xf)   + '0';
        if ( str1 > '9' )   str1 += 7;  // 'A' - '10'
        if ( str2 > '9' )   str2 += 7;        
        str += str1;
        str += str2;
    }
    
    return str;
}

}   // 命名空间tail
