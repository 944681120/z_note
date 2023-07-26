#include "MqttClt.h"

/// @brief 用法：MqttClient* pThis = (MqttClt*)((size_t)client - MqttClt::clientOffsetof);
size_t MqttClt::clientOffsetof = 0;

MqttClt::MqttClt()
{
    this->m.pClient = mqtt_lease();
    clientOffsetof = (size_t)this->m.pClient - (size_t)this;
}

MqttClt::~MqttClt()
{
    if ( this->m.pClient != nullptr )
    {
        this->disconnect();
        this->m.pClient = nullptr;
    }
}

int MqttClt::connect(char* brokerAddr, char* brokerPort, char* userName, char* password, char* clientID)
{
    mqtt_set_host(this->m.pClient, brokerAddr);
    mqtt_set_port(this->m.pClient, brokerPort);
    if ( userName != NULL && password != NULL )
    {
        mqtt_set_user_name(this->m.pClient, userName);
        mqtt_set_password(this->m.pClient, password);
    }
    ( clientID == NULL ) ? mqtt_set_client_id(this->m.pClient, (char*)"") : mqtt_set_client_id(this->m.pClient, clientID);
    mqtt_set_clean_session(this->m.pClient, 1);

    return mqtt_connect(this->m.pClient);
}

int MqttClt::disconnect()
{
    mqtt_disconnect(this->m.pClient);
    return 0;
}

int MqttClt::setSubHandler(const char* topic_filter, message_handler_t msg_handler, mqtt_qos_t qos)
{
    return mqtt_subscribe(this->m.pClient, topic_filter, qos, msg_handler);
}

int MqttClt::delSubHandler(const char *topic_filter)
{
    return mqtt_unsubscribe(this->m.pClient, topic_filter);
}

int MqttClt::setPubHandler(const char* topic_filter, const char* topic_message, mqtt_qos_t qos)
{
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));

    msg.qos = qos;
    msg.payload = (void *)topic_message;

    return mqtt_publish(this->m.pClient, topic_filter, &msg);
}

int MqttClt::sendData(mqtt_qos_t qos)
{
    if ( this->m.send.size() > 0 )  
    {
        return this->setPubHandler(this->m.send.front().topic.c_str(), this->m.send.front().buff.front().c_str(), qos);
    }

    return 0;
}

//删除缓冲的第一个数据
int MqttClt::popSendData()
{
    if ( this->m.send.size() == 0 )
    {
        return -1;
    }
    
    if ( this->m.send.front().buff.size() > 0 )
    {
        this->m.send.front().buff.pop_front();
        if ( this->m.send.front().buff.size() == 0 )
        {
            this->m.send.pop_front();
        }
        return 0;
    }
    
    return -2;  // ???返回此值不应该存在
}

int MqttClt::recvData(mqttMsg_t& oMmg)
{
    if ( this->m.recv.size() == 0 )
    {
        return -1;
    }

    oMmg = this->m.recv.front();
    return 0;
}

int MqttClt::popRecvData()
{
    if ( this->m.recv.size() == 0 )
    {
        return -1;
    }
    this->m.recv.pop_front();
    return 0;
}

int MqttClt::sendAllData(mqtt_qos_t qos)
{
    if ( this->m.send.size() == 0 )  
    {
        return -1;
    }

    for (auto &&mmg : this->m.send)
    {
        for (list<string>::iterator it = mmg.buff.begin(); it != mmg.buff.end(); ++it)
        {
            this->setPubHandler(mmg.topic.c_str(), it->c_str(), qos);
        }   
    }   
        
    return 0;
}

int MqttClt::popSendAllData()
{
    this->m.send.clear();
    return 0;
}

int MqttClt::recvAllData(list<mqttMsg_t>& oLMmg)
{
    oLMmg.clear();
    oLMmg = this->m.recv;
    return oLMmg.size();
}

int MqttClt::popRecvAllData()
{
    this->m.recv.clear();
    return 0;
}

int MqttClt::pushSendData(const char* topic_filter, const char* buf)
{
    if (topic_filter ==  nullptr || buf == nullptr)
    {
        return -1;
    }
    
    for (auto &&mbs : this->m.send)
    {
        if ( mbs.topic.compare(topic_filter) == 0 )
        {
            mbs.buff.push_back(buf);
            return 1;   // 已有
        }
    }

    MqttBuff_S newMbs;
    newMbs.topic = topic_filter;
    newMbs.buff.push_back(buf);
    this->m.send.push_back(newMbs);

    return 0;
}

int MqttClt::pushRecvData(const char* topic_filter, const char* buf)
{
    if (topic_filter ==  nullptr || buf == nullptr)
    {
        return -1;
    }
    
    for (auto &&mbs : this->m.recv)
    {
        if ( mbs.topic.compare(topic_filter) == 0 )
        {
            mbs.buff.push_back(buf);
            return 1;   // 已有
        }
    }

    MqttBuff_S newMbs;
    newMbs.topic = topic_filter;
    newMbs.buff.push_back(buf);
    this->m.recv.push_back(newMbs);

    return 0;
}
