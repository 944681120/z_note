#ifndef MQTTCLT_H
#define MQTTCLT_H
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <vector>
#include <list>
#include <string>
#include "mqttclient.h"

using namespace std;

typedef struct MqttBuff_S
{
    string topic;
    list<string> buff;
}mqttMsg_t;

struct MqttClient_S
{
    mqtt_client_t* pClient;
    list<mqttMsg_t> recv;
    list<mqttMsg_t> send;
};

class MqttClt
{
public:
    MqttClt();
    ~MqttClt();

    int connect(char* brokerAddr, char* brokerPort = (char*)"1883", char* userName = NULL, char* password = NULL, char* clientID = NULL);
    int disconnect();

    /// @brief 订阅主题处理函数，收到数据回调msg_handler
    /// @param topic_filter : 主题
    /// @param msg_handler  : 回调函数
    /// @param qos
    /// @return 成功0
    int setSubHandler(const char* topic_filter, message_handler_t msg_handler, mqtt_qos_t qos = QOS0);  //订阅
    int delSubHandler(const char* topic_filter);    //取消订阅
    int setPubHandler(const char* topic_filter, const char* topic_message, mqtt_qos_t qos = QOS0);

    /// @brief 压入发送队列，调用sendData或者sendAllData发送
    /// @param topic_filter : 主题
    /// @param buf : 数据
    /// @return 成功返回0(新主题),1(已有主题)
    int pushSendData(const char* topic_filter, const char* buf);
    
    /// @brief 压入接收队列，调用recvData或者recvAllData接收
    /// @param topic_filter : 主题
    /// @param buf : 数据
    /// @return 成功返回0(新主题),1(已有主题)
    int pushRecvData(const char* topic_filter, const char* buf);
    int sendData(mqtt_qos_t qos = QOS0);            //发送缓冲列表的第一个数据
    int sendAllData(mqtt_qos_t qos = QOS0);         //发送缓冲的全部数据
    int popSendData();                              //删除发送缓冲的第一个数据
    int popSendAllData();                           //删除发送缓冲的全部数据
    int recvData(mqttMsg_t& oMmg);                  //获取接收缓冲列表的第一列数据
    int recvAllData(list<mqttMsg_t>& oLMmg);        //获取接收缓冲的全部数据
    int popRecvData();                              //删除接收缓冲列表的第一列数据
    int popRecvAllData();                           //删除发送接收的全部数据

public:
    static size_t clientOffsetof;   //类成员pClient到类首地址的偏移，因为mqtt库没法传入this，所以使用这个值来获取this

private:
    MqttClient_S m;
};

#endif