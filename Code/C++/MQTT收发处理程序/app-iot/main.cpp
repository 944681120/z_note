#include "main.h"
long int main_t::offset_L = 0;

static int testLua(lua_State* Laa)
{
    if ( lua_isstring(Laa, 1) )
    {
        main_t* myL = (main_t*)Laa + main_t::offset_L;
        myL->str += lua_tostring(Laa, 1); // 获取Lua栈上的第一个参数作字符串
        return 0;
    }
    
    return -1;
}

int main(int argc, char *argv[])
{
    appHelp<int>(argc, argv);
    IotProject *iot = new IotProject();
    /* lua */
    main_t lua_team;
    lua_team.L = luaL_newstate(); // 创建lua运行环境
    lua_team.offset_L = &lua_team - (main_t*)(lua_team.L);
    luaL_openlibs(lua_team.L);
    lua_register(lua_team.L, "myFunction", testLua);

    /*********** 守护进程 ***********/
    iot->deamonWork("./Daemon/test/deamon.json");
    /*********** 接收mqtt数据 ***********/
    // iot->mqttClientHandler("47.115.228.188", "9091", "20230705104255", "djirenclkvwoengklfhgabnb121", "1676421655949611008")
    iot->mqttClientWork("127.0.0.1", "1883", nullptr, nullptr, nullptr);

    LLZ::Serialport usrt("/dev/ttyS1");
    usrt.setComAtrr(9600);
    while (1)
    {
        list<vector<unsigned char>> recvData;
        iot->jsonAnalysisWork();
        if ( lua_team.str.size() > 0 )
        {
            usrt.pushSend(lua_team.str.c_str(), lua_team.str.size());
            while ( usrt.send() )
            {
                ;
            }
             
            usrt.popSend();
            lua_team.str.clear();
        }

        usrt.recv();
        if ( usrt.getAllRecv(recvData) > 0) 
        {
            usrt.popAllRecv();
            for (auto &&iii : recvData)
            {
                PRI_INF("收到串口数据:%s", iii.data());     
                iii.clear();           
            }
        }
        
        sleep(1);

        luaL_loadfile(lua_team.L, "/mnt/hgfs/share/work/app-iot/Lua/script/uart1_send.lua"); // 加载lua脚本文件
        lua_pcall(lua_team.L, 0, 0, 0);
    }
}