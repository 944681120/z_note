#include "main.h"

static const char *str_opt = "vVh?";
static const char *str_help = "Usage: %s [-v,V] [-?] [-h]";
#define APP_VERSION "V1.0.0"

int appHelp(int argc, char *argv[])
{
    int ret = getopt(argc, argv, str_opt);

    while ( -1 != ret )
    {
        switch (ret) {
        case 'v':
        case 'V':
            printf("%s\n", APP_VERSION);
            exit(0);

        case 'h':
        case '?':
            printf(str_help, argv[0]);
            exit(0);

        default:
            break;
        }
        ret = getopt(argc, argv, str_opt);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    appHelp(argc, argv);
    IotProject* iot = new IotProject();
    /*********** 守护进程 ***********/
    iot->deamonWork("./Daemon/test/deamon.json");
    /*********** 接收mqtt数据 ***********/
    // iot->mqttClientHandler("47.115.228.188", "9091", "20230705104255", "djirenclkvwoengklfhgabnb121", "1676421655949611008")
    iot->mqttClientWork("127.0.0.1", "1883", nullptr, nullptr, nullptr);

    while (1)
    {        
        iot->jsonAnalysisWork();
        sleep(1);
    }
}