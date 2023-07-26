#include "MqttCltHandle.h"
#include <fstream>
#include <sstream>
#include <exception> 

using namespace LLZ;

#define ONEDAYSECS 86400


mqttCltHandle::mqttCltHandle(const char* filePath)
{
    // setUpTime(30);
    // if (filePath == NULL)
    // {
    //     initMqttCfg(MQTT_CFG);
    //     return;
    // }
    // initMqttCfg(filePath);

    AbsCfgParser* json = CfgFactory::newObj("yyjson", filePath);
    json->cfg_get_str("/param/mqtt");
    CfgFactory::delObj(json);
}

mqttCltHandle::mqttCltHandle(const char *str, int strLen)
{
}

mqttCltHandle::mqttCltHandle(const char *host, int port, const char *clientid, const char *user, const char *passwd)
{
}

mqttCltHandle::~mqttCltHandle()
{

}

const list<MqttClientConfig>& mqttCltHandle::getCfg() const
{
    return this->m.listCliCfg;
}

int mqttCltHandle::initMqttCfg(const char* filePath)
{
    if ( common_tool::isJsonFile(filePath, &jCfg) < 0 )
    {
        ERROR("以json格式读取配置文件[%s]失败", filePath);
        return -1;
    }

    int mqttCnt = jCfg["param"]["mqtt"].size();
    if( mqttCnt <= 0 )
    {
        ERROR("mqtt配置上传个数[%d]小于等于0", filePath, mqttCnt);
        return -1;
    }

    for (int i = 0; i < mqttCnt; i++)
    {
        struct MqttClientConfig tmpMcc;
        tmpMcc.host             = jCfg["param"]["mqtt"].at(i)["host"].as_string();
        tmpMcc.port             = jCfg["param"]["mqtt"].at(i)["port"].as_integer();
        tmpMcc.clientid         = jCfg["param"]["mqtt"].at(i)["clientid"].as_string();
        tmpMcc.user             = jCfg["param"]["mqtt"].at(i)["user"].as_string();
        tmpMcc.passwd           = jCfg["param"]["mqtt"].at(i)["passwd"].as_string();
        tmpMcc.topic_up         = jCfg["param"]["mqtt"].at(i)["topic_up"].as_string();
        tmpMcc.topic_up_ack     = jCfg["param"]["mqtt"].at(i)["topic_up_ack"].as_string();
        tmpMcc.topic_dn         = jCfg["param"]["mqtt"].at(i)["topic_dn"].as_string();
        tmpMcc.topic_dn_ack     = jCfg["param"]["mqtt"].at(i)["topic_dn_ack"].as_string();

        this->m.listCliCfg.push_back(tmpMcc);
    }
    
    return 0;
}

int mqttCltHandle::setUpTime(int sec)
{
    this->m.upTime = sec;
    return sec;
}

int mqttCltHandle::setClock(int sec)
{
    this->m.clock = sec;
    return sec;
}

int mqttCltHandle::clockWalk()  
{
    if ( ++this->m.clock == ONEDAYSECS )
    {
        this->m.clock = 0;
    }

    if ( this->m.clock % this->m.upTime == 0 )
    {
        this->setEventContain(MqttUpEvent::uptimeOut);
    }

    return m.clock;
}

bool mqttCltHandle::isEventContain(int event)
{
    return ( event & this->m.event );
}

int mqttCltHandle::setEventContain(int event)
{
    this->m.event |= event;
    return this->m.event;
}

int mqttCltHandle::clearEvent(MqttUpEvent event)
{
    this->m.event &= ~event;
    return 0;
}

bool mqttCltHandle::isEventContainAndClearEvent(int event)
{
    bool ret = this->isEventContain(event);
    this->clearEvent((MqttUpEvent)event);
    return ret;
}

int mqttCltHandle::analysis(void* arg, void* (*handler)(void* arg), bool isThread)
{
    if ( handler == NULL )
    {
        return 0;
    }

    if ( isThread == true )
    {
        common_tool::creatThread(handler, arg);
    }
    else
    {
        handler(arg);
    }
    return 0;
}