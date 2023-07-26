#ifndef IOTPROJECT_H
#define IOTPROJECT_H

#include "main.h"

using pcr_c = const char*;
using namespace LLZ;

class IotProject
{
public:
    IotProject();
    ~IotProject();

    int deamonWork(const char* filePath);
    int mqttClientWork(pcr_c brokerAddr, pcr_c brokerPort, pcr_c userName, pcr_c password, pcr_c clientID);
    int jsonAnalysisWork();

    int appOptHandler();
    int sshdSetHandler();
    int serialCmdHandler();
    int proxyHandler(const char* filePath);

private:
    struct IotProject_s
    {
        Daemon  daem;
        MqttClt mqttc;
        JsonPs  jsonp;
        IniPs   inip;
        map<string, string> daemAppMsg;
    }*m = new IotProject_s();
};

#endif