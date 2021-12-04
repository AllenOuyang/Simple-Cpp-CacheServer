#include "TcpSocket.hpp"
#include "CacheServer.hpp"

int main()
{
    // 创建CacheServer，设置最大容量为100
    CacheServer m_Cache(100);
    // 启动CacheServer
    m_Cache.run();

    return 0;

    // 以下为测试代码，用于测试简单的通信
    // // 1. 创建监听的套接字，指定LRUCache的大小为100
    // CacheServer s(100);
    // // 2. 绑定本地的IP port并设置监听
    // s.setListen(10000);
    // // 3. 阻塞并等待客户端的连接
    // while (1)
    // {
    //     SockInfo* info = new SockInfo;
    //     TcpSocket* tcp = s.acceptConn(&info->addr);
    //     if (tcp == nullptr)
    //     {
    //         std::cout << "重试...." << std::endl;
    //         continue;
    //     }
    //     // 创建子线程
    //     pthread_t tid;
    //     info->s = &s;
    //     info->tcp = tcp;

    //     pthread_create(&tid, NULL, working, info);
    //     pthread_detach(tid);
    // }

    // return 0;
}