#include "Daemon.h"
#include "zcom.h"
#include <mutex>
#include <iostream>

std::thread Daemon::threads;

Daemon::Daemon()
{
    thrRun();
}

Daemon::~Daemon()
{

}

int Daemon::daemByProgram(const char* program, const char* restartCmd, bool defaultOpen)
{
    if ( program == nullptr )
    {
        return 0;
    }

    if ( this->m->deamList.count(program) == 0 )
    {
        prgInf_s tmp;
        tmp.restartCmd = restartCmd;
        tmp.name = program;
        ( defaultOpen == true ) ? tmp.state = 1 : tmp.state = -1;
        this->m->deamList[program] = tmp;
    }
    return 0;
}

int Daemon::killByProgram(const char* program)
{
    if ( this->m->deamList.count(program) == 0 || this->m->deamList[program].pid == -1 )
    {
        return 0;
    }
    
    string killCmd = "kill -9 " + to_string(this->m->deamList[program].pid);
    this->m->deamList[program].state = 0;
    this->m->deamList[program].pid = -1;
    system(killCmd.c_str());
    return 0;
}

int Daemon::goOnByProgram(const char *program)
{
    if ( this->m->deamList.count(program) == 0 )
    {
        return 0;
    }
    this->m->deamList[program].state = 1;
    
    return 0;
}

int Daemon::thrRun()
{
    this->threads = std::thread([](Daemon* arg) 
    {
        while (1)
        {
            for (map<string, prgInf_s>::iterator it = arg->m->deamList.begin(); it != arg->m->deamList.end(); ++it)
            {
                /* 程序没有运行,则拉起程序 */
                if ( ((*it).second.pid = LLZ::detect_process((*it).first.c_str())) < 0 )
                {
                    if ( (*it).second.state == 2 )
                    {
                        (*it).second.state++;
                        PRI_ERR("APP:%s拉起失败", (*it).first.c_str());
                    }
                    
                    if ( (*it).second.state == 1 )
                    {
                        (*it).second.state++;
                        PRI_INF("RUN APP:%s, CMD:%s", (*it).first.c_str(), (*it).second.restartCmd.c_str());
                        /* 考虑可移植性没使用fork() */
                        std::thread childThr = std::thread([it]()
                        {
                            system((*it).second.restartCmd.c_str());
                        });
                        childThr.detach();
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }, this);
    this->threads.detach();

    return 0;
}
