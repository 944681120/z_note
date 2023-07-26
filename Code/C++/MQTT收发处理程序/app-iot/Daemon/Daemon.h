#ifndef DAEMON_H
#define DAEMON_H

#pragma once
#include <map>
#include <list>
#include <vector>
#include <string>
#include <thread>

using namespace std;

typedef struct programInfo_s 
{
    int pid = -1;
    // int daemTime = -1;   // 守护时间
    int state = -1; // 启动个数
    string name;
    string restartCmd;
}prgInf_s;

class Daemon
{
public:
    Daemon();
    ~Daemon();

    /// @brief              ：通过程序名守护程序, 程序挂了则执行cmd命令
    /// @param program      : 程序名
    /// @param restartCmd   ：完整的重启命令
    /// @param defaultOpen  : 初始状态，默认不开启
    int daemByProgram(const char* program, const char* restartCmd, bool defaultOpen = false);
    int killByProgram(const char* program);
    int goOnByProgram(const char* program);

private:
    int thrRun();

private:
    static std::thread threads;
    struct Daemon_s
    {
        map<string, prgInf_s> deamList;
    }*m = new Daemon_s();
};

#endif