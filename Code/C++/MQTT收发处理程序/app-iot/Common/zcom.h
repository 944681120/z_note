#ifndef ZCOM_H
#define ZCOM_H

#include <string.h>
#include <string>
#include <chrono>
#include <thread>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define     PRI(fmt, ...)  printf("[%s:%02d] "       fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define PRI_INF(fmt, ...)  printf("[%s:%02d][INF]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define PRI_MSG(fmt, ...)  printf("[%s:%02d][MSG]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define PRI_ERR(fmt, ...)  printf("[%s:%02d][ERR]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#ifdef CMAKE_BUILD_TYPE
    #define PRI_DEBUG(fmt, ...)  printf("[%s_%02d] "       fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#else
    #define PRI_DEBUG(fmt, ...)
#endif // Debug

using namespace std;
namespace LLZ
{
    /* 判断进程是否在运行，RETURN：PID 或 -1 */
    int detect_process(const char* process_name);
    /* kill掉进程 */
    int kill_process(const char* process_name);
    /* 开启一个线程 */
    int creatThread(void* (*pFunc)(void*), void* arg = nullptr);
    /* HEX转字符串 */
    string hexToStr(const unsigned char* hexBuf, int bufLen);
};

#endif