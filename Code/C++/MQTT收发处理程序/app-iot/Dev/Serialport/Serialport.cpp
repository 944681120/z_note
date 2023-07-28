#include "Serialport.h"

#include "termios.h"
#include "fcntl.h"
#include "unistd.h"

using namespace LLZ;

Serialport::Serialport(const char* comDev)
{
    this->m = new Serialport_s();
    this->openDev(comDev);
    this->setComAtrr(9600);
}
Serialport::~Serialport()
{
    this->closeDev();
    if ( this->m != NULL )
    {
        delete this->m;
        this->m = NULL;
    }
}

int Serialport::setComAtrr(int speed, int flow_ctrl, int databits, int stopbits, int parity)
{
    int ret;
    int speed_arr[] = {B115200, B19200, B9600, B4800, B2400, B1200, B300};
    int name_arr[] = {115200, 19200, 9600, 4800, 2400, 1200, 300};
    struct termios options;

    if ( this->m->fd < 0)
    {
        printf("fd Error! 请先openDev()\n");
        return -1;
    }

    ret = tcgetattr(this->m->fd, &options);
    if (ret != 0)
    {
        printf("tcgetattr() Error! 获取串口设备节点属性失败\n");
        return -1;
    }

    /* 设置输入波特率和输出波特率 */
    for (int i = 0; i < sizeof(speed_arr) / sizeof(speed_arr[0]); i++)
    {
        if (speed == name_arr[i])
        {
            cfsetispeed(&options, speed_arr[i]);
            cfsetospeed(&options, speed_arr[i]);
            break;
        }
    }

    /* 修改控制模式，保证程序不会占用串口 */
    options.c_cflag |= CLOCAL;

    /* 修改控制模式，使得能够从串口中读取输入数据 */
    options.c_cflag |= CREAD;

    /* 设置数据流控制 */
    options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    switch (flow_ctrl)
    {
        case 0: /* 不使用流控制 */
            options.c_cflag &= ~CRTSCTS;
            break;
        case 1: /* 使用硬件流控制 */
            options.c_cflag |= CRTSCTS;
            break;
        case 2: /* 使用软件流控制 */
            options.c_cflag |= IXON | IXOFF | IXANY;
            break;

        default:
            printf("Unsupported flow_ctrl\n");
            options.c_cflag &= ~CRTSCTS;
            break;
    }

    /* 设置数据位 */
    /* 屏蔽其他标志位 */
    options.c_cflag &= ~CSIZE;
    switch (databits)
    {
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;

        default:
            printf("Unsupported data size\n");
            options.c_cflag |= CS8;
            break;
    }

    /* 设置校验位 */
    switch (parity)
    {
        case 0: /* 无奇偶校验位 */
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
        case 1: /* 设置为奇校验 */
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= INPCK;
            break;
        case 2: /* 设置为偶校验 */
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            break;
        case 3: /* 设置为空格 */
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;

        default:
            printf("Unsupported parity\n");
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
    }

    /* 设置停止位 */
    switch (stopbits)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;

        default:
             printf("Unsupported stop bits\n");
            options.c_cflag &= ~CSTOPB;
            break;
    }

    /* 修改输出模式，原始数据输出 */
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* 设置等待时间和最小接收字符 */
    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN] = 1;  /* 读取字符的最少个数为1 */

    /* 如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读 */
    tcflush(this->m->fd, TCIFLUSH);

    /* 激活配置 (将修改后的termios数据设置到串口中） */
    ret = tcsetattr(this->m->fd, TCSANOW, &options);
    if (ret != 0)
    {
        printf("tcsetattr() Error! 设置串口属性失败\n");
        return -1;
    }

    return 0;
}

int Serialport::resetComAtrr(const char* comDev, int speed, int flow_ctrl, int databits, int stopbits, int parity)
{
    if ( this->m->COM.compare(comDev) != 0 )
    {
        if ( this->openDev(comDev) < 0)
        {
            return -1;
        }
    }

    return setComAtrr(speed, flow_ctrl, databits, stopbits, parity);
}

int Serialport::pushSend(const char* data, int dataLen)
{
    if (data == NULL || data[0] == 0)
    {
        return -1;
    }

    vector<unsigned char> tmpSend;
    for (int i = 0; i < dataLen; i++)
    {
        tmpSend.push_back(data[i]);
    }
    this->m->sendBuf.push_back(tmpSend);

    return dataLen;
}

int Serialport::pushRecv(const char* data, int dataLen)
{
    if (data == NULL)
    {
        return -1;
    }

    vector<unsigned char> tmpRecv;
    for (int i = 0; i < dataLen; i++)
    {
        tmpRecv.push_back(data[i]);
    }
    this->m->recvBuf.push_back(tmpRecv);

    return dataLen;
}

inline int Serialport::getBuf(char* data, int dataLen, char chrSendOrRecv)
{
    if (data == NULL)
    {
        return -1;
    }

    vector<unsigned char> tmpBuf;
    switch ( chrSendOrRecv )
    {
        case 'S':
        case 's':
            if ( this->m->sendBuf.size() > 0 )
            {
                tmpBuf = this->m->sendBuf.front();
            }
            break;

        case 'R':
        case 'r':
            if ( this->m->recvBuf.size() > 0 )
            {
                tmpBuf = this->m->recvBuf.front();
            }
            break;
    
        default:
            return -2;
    }

    int len = tmpBuf.size();
    if( len == 0)
    {
        return 0;
    }
    if( len > dataLen)
    {
        len = dataLen;
    }

    for (int i = 0; i < len; i++)
    {
        data[i] = tmpBuf.at(i);
    }
    
    return len;
}

int Serialport::getSend(char* data, int dataLen)
{
    return this->getBuf(data, dataLen, 's');
}

int Serialport::getRecv(char* data, int dataLen)
{
    return this->getBuf(data, dataLen, 'r');
}

int Serialport::getAllRecv(list<vector<unsigned char>>& data)
{
    int ret = this->m->recvBuf.size();
    if ( ret > 0 )
    {
        data = this->m->recvBuf;
        return ret;
    }

    return -1;
}

int Serialport::send()
{
    if( this->m->sendBuf.size() > 0 )
    {
        vector<unsigned char> buf = this->m->sendBuf.front();
        printf("发送数据[%ld]:", buf.size());
        for (int i = 0; i < buf.size(); i++)
        {
            printf("%c", buf[i]);
        }
        printf("\n");
        
        this->ctrl(0);
        int ret = write(this->m->fd, &this->m->sendBuf.front()[0], this->m->sendBuf.front().size());
        this->ctrl(1);
        if ( ret < 0 )
        {
            printf("发送数据失败 send::write():%d\n", ret);
        }
        else
        {
            this->m->sendBuf.pop_front();
            return ret;
        }
    }

    return 0;
}

int Serialport::popSend()
{
    if( this->m->sendBuf.size() )
    {
        this->m->sendBuf.pop_front();
    }
    return 0;
}

int Serialport::popRecv()
{
    if( this->m->recvBuf.size() )
    {
        this->m->recvBuf.pop_front();
    }
    return 0;
}

int LLZ::Serialport::popAllRecv()
{
    this->m->recvBuf.clear();
    return 0;
}

int Serialport::recv()
{
    unsigned char buf[16] = {0};
    int ret = 0;
    vector<unsigned char> recvTmp;
    do
    {
        ret = read(this->m->fd, (void*)buf, sizeof(buf));
        if( ret > 0)
        {
            recvTmp.insert(recvTmp.end(), (unsigned char*)buf, (unsigned char*)buf + ret);
        }
    }while(ret > 0);

    if ( recvTmp.size() > 0 )
    {       
        this->m->recvBuf.push_back(recvTmp);
    }

    return 0;
}

int Serialport::recvSendThread()
{    
    std::thread detachedThread([](void* ptr)->void*
    {
        Serialport* pThis = (Serialport*)ptr;
        printf("创建recvSendThread成功");
        for (;;)
        {
            pThis->send();
            pThis->recv();
            usleep(1000);
        }

        printf("recvSendThread退出");
        return NULL;
    }, this);
    detachedThread.detach();

    return 0;
}

static int openCtrlFile(int index)
{
    static int crlfd1 = open("/dev/rs485Ctrl1", O_RDWR | O_NOCTTY | O_NONBLOCK);
    static int crlfd2 = open("/dev/rs485Ctrl2", O_RDWR | O_NOCTTY | O_NONBLOCK);

    switch (index)
    {
    case 1:
        return crlfd1;

    case 2:
        return crlfd2;

    default:
        break;
    }

    return -1;
}

int Serialport::ctrl(int value)
{
    // char ctrlvalue = (char)value;
    // char ctrlPortIndex = 0;
    // const char *io[] = {"/dev/rs485Ctrl1", "/dev/rs485Ctrl2"};
    // if (strcmp(this->m->COM.c_str(), "/dev/ttyRS1") == 0)
    // {
    //     ctrlPortIndex = 1;
    // }
    // if (strcmp(this->m->COM.c_str(), "/dev/ttyRS2") == 0)
    // {
    //     ctrlPortIndex = 2;
    // }
    // if (ctrlPortIndex == 0)
    // {
    //     printf("ctrl(%d) ERR! 没找到相应设备节点\n", value);
    //     return -1;
    // }

    // int crlfd = openCtrlFile(ctrlPortIndex);
    // if (crlfd < 0)
    // {
    //     printf("ctrl()::openCtrlFile(%d) ERR! 打开控制收发节点失败\n", ctrlPortIndex);
    //     return -1;
    // }

    // int iDataLen = write(crlfd, &ctrlvalue, sizeof(ctrlvalue));
    // if (iDataLen < 0)
    // {
    //     printf("ctrl()::write() ERR! 写控制收发节点失败\n", ctrlPortIndex);
    //     return -1;
    // }

    return 0;
}

int Serialport::openDev(const char* comDev)
{
    int fd = -1;
    this->closeDev();
    fd = open(comDev, O_RDWR | O_NOCTTY | O_SYNC | O_NDELAY | O_NONBLOCK );
    if (fd == -1)
    {
        printf("[Serialport]: openDev %s error...\n", comDev);
        return (-1);
    }
    
    this->m->fd = fd;
    this->m->COM = comDev;

    return fd;
}

int Serialport::closeDev()
{
    if ( this->m->fd >= 0)
    {
        close(this->m->fd);
        this->m->fd = -1;
        this->m->COM.clear();
    }
    return 0;
}
