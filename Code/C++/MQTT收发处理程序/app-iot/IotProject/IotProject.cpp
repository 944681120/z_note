#include "IotProject.h"

IotProject::IotProject()
{

}

IotProject::~IotProject()
{

}

int IotProject::serialCmdHandler()
{
    return 0;
}

int IotProject::sshdSetHandler()
{
    return 0;
}

int IotProject::proxyHandler(const char* filePath)
{
    string  str;
    string  switch_ste;         // 代理开关
    string  remote_ip;          // 代理ip
    int     remote_port = -1;   // 代理端口
    int     proxy_port = -1;    // 本地端口

    int num = this->m->jsonp.cfg_arr_iter("/inputs", nullptr);
    for (int i = 0; i < num; i++)
    {
        string namePath = "/inputs/" +  to_string(i) + "/name";
        string valuePath = "/inputs/" +  to_string(i) + "/value";
        this->m->jsonp.cfg_get_str(namePath.c_str(), str);
        if ( str.compare("switch") == 0 )
        {
            this->m->jsonp.cfg_get_str(valuePath.c_str(), switch_ste);
        }
        if ( str.compare("proxy_port") == 0 )
        {
            this->m->jsonp.cfg_get_int(valuePath.c_str(), proxy_port);
        }
        if ( str.compare("remote_ip") == 0 )
        {
            this->m->jsonp.cfg_get_str(valuePath.c_str(), remote_ip);
        }
        if ( str.compare("remote_port") == 0 )
        {
            this->m->jsonp.cfg_get_int(valuePath.c_str(), remote_port);
        }
    }

    /* 关闭代理 */
    if ( switch_ste.compare("close") == 0 )
    {
        /* kill掉程序 */
        PRI_MSG("closed proxy");
        this->m->daem.killByProgram("frpc");
        return 0;
    }
    PRI_INF("代理设置:%s 代理端口:%d", switch_ste.c_str(), proxy_port);
    PRI_INF("被代理端口 %s:%d\n", remote_ip.c_str(), remote_port);

    if ( remote_port != -1 && switch_ste.compare("open") == 0 )
    {
        /* kill掉程序 */
        this->m->daem.killByProgram("frpc");

        /* 修改配置文件 */
        list<string> strList;
        this->m->inip.open(filePath);
        int num = this->m->inip.cfg_arr_iter("/", &strList);
        /* 修改sections */
        str.clear();
        for (auto&& mstr : strList)
        {
            if ( mstr.find("ssh-") != mstr.npos )
            {
                str = mstr.c_str();
                break;
            }
        }
        if ( str.empty() )
        {
            PRI_ERR("frc.ini没有找到ssh的sections");
            return -1;
        }
        
        this->m->inip.cfg_set_int((str + "/remote_port").c_str(), remote_port);
        string newstr = this->m->inip.cfg_print_str();
        int pos = newstr.find(str.c_str());
        if ( pos > 0 )
        {
            newstr.replace(pos, str.size(), string("ssh-") + to_string(remote_port) );
        }
        this->m->inip.open(newstr.c_str(), newstr.size());
        this->m->inip.cfg_save_file(filePath);
        
        LLZ::creatThread([](void* args)->void*
        {
            /* 小段延时后拉起配置文件 */
            IotProject* pThis = (IotProject*)args;
            std::this_thread::sleep_for(chrono::seconds(5));
            pThis->m->daem.goOnByProgram("frpc");
            return nullptr;
        }, this);
    }
    return 0;
}

int IotProject::appOptHandler()
{
    string  str;
    string  node;
    string  operate;
    int num = this->m->jsonp.cfg_arr_iter("/inputs", nullptr);
    for (int i = 0; i < num; i++)
    {
        string namePath = "/inputs/" +  to_string(i) + "/name";
        string valuePath = "/inputs/" +  to_string(i) + "/value";
        this->m->jsonp.cfg_get_str(namePath.c_str(), str);
        if ( str.compare("node") == 0 )
        {
            this->m->jsonp.cfg_get_str(valuePath.c_str(), node);
        }
        if ( str.compare("operate") == 0 )
        {
            this->m->jsonp.cfg_get_str(valuePath.c_str(), operate);
        }
    }

    if ( operate.compare("reboot") == 0 )
    {
        LLZ::kill_process(node.c_str());
        if ( this->m->daemAppMsg.count(node) == 0 )
        {
            system(node.c_str());   // 不在守护列表的直接运行
        }
        else
        {
            system(this->m->daemAppMsg[node].c_str());
        }
        PRI_INF("重启APP:%s", node.c_str());
    }
    

    return 0;
}

int IotProject::deamonWork(const char *filePath)
{
    /*********** 守护进程 ***********/
    this->m->jsonp.open(filePath);

    /* 获取全部的key值，再获取全部的value值 */
    list<string> keys, vals;
    this->m->jsonp.cfg_arr_iter("/app_list/default_close", &keys);    // key
    for (auto &&key : keys)
    {
        string str("/app_list/default_close/" + key);         // path
        this->m->jsonp.cfg_get_str(str.c_str(), str);   // val
        PRI_INF("default_close >> %s : %s", key.c_str(), str.c_str());
        vals.push_back(str);
    }
    /* 创建守护进程对象 */
    for (auto &&key : keys)
    {
        this->m->daem.daemByProgram(key.c_str(), vals.front().c_str(), false);
        this->m->daemAppMsg[key] = vals.front();
        vals.pop_front();
    }

    keys.clear();
    this->m->jsonp.cfg_arr_iter("/app_list/default_open", &keys);    // key
    for (auto &&key : keys)
    {
        string str("/app_list/default_open/" + key);         // path
        this->m->jsonp.cfg_get_str(str.c_str(), str);   // val
        PRI_INF("default_open >> %s : %s", key.c_str(), str.c_str());
        vals.push_back(str);
    }
    /* 创建守护进程对象 */
    for (auto &&key : keys)
    {
        this->m->daem.daemByProgram(key.c_str(), vals.front().c_str(), true);
        this->m->daemAppMsg[key] = vals.front();
        vals.pop_front();
    }
    return 0;
}

int IotProject::mqttClientWork(pcr_c brokerAddr, pcr_c brokerPort, pcr_c userName, pcr_c password, pcr_c clientID)
{
    if ( this->m->mqttc.connect((char*)brokerAddr, (char*)brokerPort, (char*)userName, (char*)password, (char*)clientID) < 0 )
    {
        PRI_ERR("连接MQTT失败");
    }
    // this->m->mqttc.setSubHandler("/1676421127983206400/1676421655949611008/function/invoke", [](void* client, message_data_t* msg)
    this->m->mqttc.setSubHandler("1234", [](void* client, message_data_t* msg)
    {
        /* 终端：mosquitto_pub -t 'topic' -m "Hello World" */
        PRI_INF("MQTT收到了数据: [%s]%s\n", (char*)msg->topic_name, (char*)msg->message->payload);
        MqttClt* pTHis = (MqttClt*)((size_t)client - MqttClt::clientOffsetof);
        /* 将收到的数据压入队列 */
        pTHis->pushRecvData(msg->topic_name, (char*)msg->message->payload);
    });
    return 0;
}

/* 解析json, 业务处理 */
int IotProject::jsonAnalysisWork()
{
    mqttMsg_t mmsg;    
    if ( this->m->mqttc.recvData(mmsg) == 0 )
    {
        this->m->mqttc.popRecvData();
        string str;
        /* 主题为xxxxx的处理 */
        if ( mmsg.topic.compare("1234") == 0 )
        {
            while ( mmsg.buff.size() > 0 )
            {
                this->m->jsonp.open(mmsg.buff.front().c_str(), 1);
                mmsg.buff.pop_front();

                this->m->jsonp.cfg_get_str("/functionId", str);
                if ( str.compare("proxy_set") == 0 )
                {
                    this->proxyHandler("CfgParser/test/frpc.ini");
                }
                if ( str.compare("app_opt") == 0 )
                {
                    this->appOptHandler();
                }
                if ( str.compare("ssh") == 0 )
                {
                    this->appOptHandler();
                }
            }
            return 0;
        }
    }

    return 0;
}
