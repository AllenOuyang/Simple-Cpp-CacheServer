#include "CacheServer.hpp"

// 发送心跳包 send heartbeat package 的线程函数
void* send_heart(void *arg)
{
    std::cout << "The heartbeat sending thread started." << std::endl;
    // 1. 创建通信的套接字
    TcpSocket tcp;
    // 2. 连接服务器IP port
    int ret = tcp.connectToHost("127.0.0.1", 10000);
    // 3. 发送心跳包
    while(true)
    {
        struct HeartPackage HP;
    }
    
}


// 接受Client读写请求的线程函数
void* working(void* arg)
{
    struct SockInfo* pinfo = static_cast<struct SockInfo*>(arg);
    // 连接建立成功, 打印客户端的IP和端口信息
    char ip[32];
    printf("客户端的IP: %s, 端口: %d\n",
        inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)),
        ntohs(pinfo->addr.sin_port));

    // 5. 通信
    while (1)
    {
        printf("接收数据: .....\n");
        std::string msg = pinfo->_tcp->recvMsg();
        if (!msg.empty())
        {
            //std::cout << msg << std::endl << std::endl << std::endl;
            // 收到的msg是一个由空格分开的key value对， 我们利用stringstream来拆分ms中的key和value
            std::stringstream os;
            os << msg;
            std::string key, value;
            os >> key >> value;
            os.clear();
            // 把key value对存到LRUCache中
            pinfo->_server->put(key, value);
        }
        else
        {
            break;
        }
    }
    delete pinfo->_tcp;
    delete pinfo;
    return nullptr;
}

// 构造函数，初始化CacheServer的容量
CacheServer::CacheServer(int capacity)
{
    lru.set_capacity(capacity);
}

// 启动函数
void CacheServer::run()
{

    // 1.状态上报给Master
    // 创建子线程用于状态上报
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, send_heart, (void*)this);
    if(ret != 0)
    {
        std::cout << "Can not create thread!" << std::endl;
        exit(1);
    }

    // 2.接受Client的读写请求，服务于Client
    // 绑定本地的IP Port并设置监听
    setListen(15000);
    // 阻塞并等待客户端连接
    while (1)
    {
        SockInfo* info = new SockInfo;
        TcpSocket* tcp = acceptConn(&info->addr);
        if (tcp == nullptr)
        {
            std::cout << "重试...." << std::endl;
            continue;
        }
        // 创建子线程
        pthread_t tid;
        info->_server = this;
        info->_tcp = tcp;

        pthread_create(&tid, NULL, working, info);
        pthread_detach(tid);
    }
    
}

void CacheServer::put(std::string _key, std::string _value)
{
    lru.put(_key, _value);
}