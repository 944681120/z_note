#pragma once
#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

// 串口时钟头文件
#include <signal.h>
#include <time.h>

// 线程， 信号量头文件
#include <pthread.h>
#include <semaphore.h>

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <list>
#include <thread>

#include "Serialport.h"

using namespace std;
namespace LLZ { // 命名空间

struct Serialport_s
{
    list<vector<unsigned char>> sendBuf;
    list<vector<unsigned char>> recvBuf;
    string COM;
    int fd = -1;
};

class Serialport
{
public:
    Serialport(const char* comDev);
    ~Serialport();

    /* 控制串口收发,半双工使用 */
    int ctrl(int value);    // 0发1收
    /* 初始化串口 */
    int setComAtrr(int speed, int flow_ctrl = 0, int databits = 8, int stopbits = 1, int parity = 0);
    int resetComAtrr(const char* comDev, int speed, int flow_ctrl = 0, int databits = 8, int stopbits = 1, int parity = 0);
    /* 设置收发数据队列 */
    int pushSend(const char* data, int dataLen);
    int pushRecv(const char* data, int dataLen);
    /* 获取收发数据 */
    int getSend(char* data, int dataLen);
    int getRecv(char* data, int dataLen); 
    int getAllRecv(list<vector<unsigned char>>& data); 
    /* 弹出收发队头 */
    int popSend();
    int popRecv(); 
    int popAllRecv(); 
    /* 收发函数 */
    int send();
    int recv(); 
    /* 自动收发线程 */
    int recvSendThread();

protected:
    inline int getBuf(char* data, int dataLen, char chrSendOrRecv = 's');
    /* 打开设备节点 */
    int openDev(const char* comDev);
    int closeDev();

private:
    Serialport_s* m;
};

} // 命名空间

#endif