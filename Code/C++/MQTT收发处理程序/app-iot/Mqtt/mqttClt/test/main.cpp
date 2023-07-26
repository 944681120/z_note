#include "main.h"
#include "MqttClt.h"

int main(int argc, char const *argv[])
{
    MqttClt mqttc;
    mqttc.connect((char*)"127.0.0.1", (char*)"1883", nullptr, nullptr, nullptr);
    printf("mqttc的地址: [%lx]\n", (size_t)&mqttc);
    
    int retsub = mqttc.setSubHandler("subTopName1", [](void* client, message_data_t* msg)
    {
        /* 终端：mosquitto_pub -t subTopName1 -m "Hello World" */
        printf("\t收到了数据: [%s]%s\n", (char*)msg->topic_name, (char*)msg->message->payload);
        MqttClt* pTHis = (MqttClt*)((size_t)client - MqttClt::clientOffsetof);
        static int oneflag = 0;
        if ( oneflag == 0 )
        {
            oneflag++;
            printf("\t反推mqttc的地址: [%lx]\n", (size_t)pTHis);
        }

        /* 将收到的数据压入队列 */
        pTHis->pushRecvData(msg->topic_name, (char*)msg->message->payload);
    });

    mqttc.setSubHandler("subTopName2", [](void* client, message_data_t* msg)
    {
        /* 终端：mosquitto_pub -t subTopName2 -m "Hello World" */
        printf("\t收到了数据: [%s]%s\n", (char*)msg->topic_name, (char*)msg->message->payload);
        MqttClt* pTHis = (MqttClt*)((size_t)client - MqttClt::clientOffsetof);
        /* 将收到的数据压入队列 */
        pTHis->pushRecvData(msg->topic_name, (char*)msg->message->payload);
    });

    /* 压入还没发送 */
    mqttc.pushSendData("subTopName1", "给subTopName1主题发送的数据1");
    mqttc.pushSendData("subTopName1", "给subTopName1主题发送的数据2");
    mqttc.pushSendData("subTopName1", "给subTopName1主题发送的数据3");
    mqttc.pushSendData("subTopName2", "给subTopName2主题发送的数据4");

    /* 发送第一个数据, QOS1可能收到两次 */
    if ( mqttc.sendData(QOS0) >= 0 )
    {
        mqttc.popSendData();    //发送成功就清除一个，仅一个数据
    }

    mqtt_sleep_ms(1000);
    mqttMsg_t mmg;  //读取接收的数据
    if( mqttc.recvData(mmg) == 0 )  //接收到数据1
    {
        printf("\n换行+++++++++++++++++\n");
        for (auto &&i : mmg.buff)
        {
            printf("main 接收打印: [%s]%s\n", mmg.topic.c_str(), i.c_str());
        }
        mqttc.popRecvData();    //接收成功就清掉
    }

    printf("\n换行--------------------\n");
    /* 发送全部的数据 */
    mqttc.sendAllData();    // 发送数据2、3、4
    mqttc.popSendAllData();
    printf("发送了全部数据\n换行====================\n");
    
    mqtt_sleep_ms(1500);
    if( mqttc.recvData(mmg) == 0 )  //接收2、3
    {
        printf("\n换行+++++++++++++++++\n");
        for (auto &&i : mmg.buff)
        {
            printf("main 接收打印: [%s]%s\n", mmg.topic.c_str(), i.c_str());
        }
        mqttc.popRecvData();    //接收成功就清掉
    }

    if( mqttc.recvData(mmg) == 0 )  //接收4
    {
        printf("\n换行+++++++++++++++++\n");
        for (auto &&i : mmg.buff)
        {
            printf("main 接收打印: [%s]%s\n\n", mmg.topic.c_str(), i.c_str());
        }
        mqttc.popRecvData();    //接收成功就清掉
    }

    return 0;
}