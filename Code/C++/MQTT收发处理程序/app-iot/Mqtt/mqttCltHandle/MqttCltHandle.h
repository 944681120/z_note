#ifndef _MQTTCLTHANDLE_H_
#define _MQTTCLTHANDLE_H_

#include <unistd.h>
#include <list>
#include <string>
#include "../mqttClt/MqttClt.h"
#include "../../CfgParser/CfgFactory.h"

using namespace std;
#define MQTT_CFG "mqtt_setting.json"

namespace LLZ {    // 命名空间

struct MqttClientConfig
{
    string host;
    int port;
    string clientid;
    string user;
    string passwd;
};

typedef list<MqttClientConfig> listCliCfg_t;

struct mqttCltHandle_S
{
    int upTime;                 // 定时报时间
    int clock;                  // 类计数时钟
    int event;                  // 类事件
    listCliCfg_t listCliCfg;    // 客户端信息列表
};

enum MqttUpEvent    
{
    upOnTime    = (1 << 0),     // 定时上报
    reUpload    = (1 << 1),     // 重新上报
    upAtOnce    = (1 << 2),     // 一次上报
    Ack         = (1 << 3),     // 应答上报
};

class mqttCltHandle
{
public:
    mqttCltHandle(const char* filePath = NULL); // 配置文件方式创建对象
    mqttCltHandle(const char* str, int strLen); // 字符串的方式创建对象
    mqttCltHandle(const char* host, int port, const char* clientid, const char* user, const char* passwd);
    ~mqttCltHandle();

    int initMqttCfg(const char* filePath);
    const list<MqttClientConfig>& getCfg() const;
    int clockWalk();
    int setClock(int sec);
    bool isEventContain(int event);
    int setEventContain(int event);
    int clearEvent(MqttUpEvent event);
    bool isEventContainAndClearEvent(int event);
    int analysis(void* arg, void* (*handler)(void* arg), bool isThread = false);
    int setUpTime(int sec);

private:
    mqttCltHandle_S m;
};

}   // 命名空间

#endif // _MQTTUP_H_