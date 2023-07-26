#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "mqttclient.h"

static void subHandle1(void *client, message_data_t *msg)
{
    (void)client;
    MQTT_LOG_I("-----------------------------------------------------------------------------------");
    MQTT_LOG_I("\n%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char *)msg->message->payload);
    MQTT_LOG_I("-----------------------------------------------------------------------------------");
}

static void subHandle2(void *client, message_data_t *msg)
{
    (void)client;
    MQTT_LOG_I("-----------------------------------------------------------------------------------");
    MQTT_LOG_I("\n%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char *)msg->message->payload);
    MQTT_LOG_I("-----------------------------------------------------------------------------------");
}

static int mqtt_publish_handle1(mqtt_client_t *client)
{
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));

    msg.qos = QOS0;
    msg.payload = (void *)"sendMsg";

    return mqtt_publish(client, "sendTopName", &msg);
}

static int mqtt_publish_sub1(mqtt_client_t *client)
{
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));

    msg.qos = QOS0;
    msg.payload = (void *)"sendMsg to subTopName1";

    return mqtt_publish(client, "subTopName1", &msg);
}

int main(void)
{
    mqtt_log_init();
    mqtt_client_t *client = mqtt_lease();

    /* 需要本地创建mqtt broker */
    mqtt_set_host(client, (char*)"127.0.0.1");
    mqtt_set_port(client, (char*)"1883");
    mqtt_set_clean_session(client, 1);

    mqtt_connect(client);
    mqtt_subscribe(client, "subTopName1", QOS0, subHandle1);
    mqtt_subscribe(client, "subTopName2", QOS0, subHandle2);

    while (1)
    {
        mqtt_publish_handle1(client);
        mqtt_publish_sub1(client);
        mqtt_sleep_ms(3 * 1000);
    }
}
