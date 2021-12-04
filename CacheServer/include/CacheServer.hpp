#ifndef _CACHESERVER_HPP
#define _CACHESERVER_HPP
#include "TcpServer.hpp"
#include "TcpSocket.hpp"
#include "lru.hpp"
#include <sstream>

struct SockInfo
{
    CacheServer* _server;
    TcpSocket* _tcp;
    struct sockaddr_in addr;
};

struct HeartPackage
{
    int sizeLru;
    int CapaLru;
};

class CacheServer : public TcpServer
{
public:
    // 发送心跳包send heartbeat package 线程函数
    friend void* send_heart(void* arg);

    // 接受Client读写请求的线程函数
    friend void* working(void* arg);

    // 有参构造函数
    CacheServer(int capacity);

    // run启动函数，分两部分，第一部分CacheServer服务于CLient，
    // 第二部分状态上报（心跳上报）给Master
    void run();
    
    // 存储key-value对
    void put(std::string _key, std::string _value);
    // 暂时不知道要不要写析构函数

public:
    // 利用LRUCache实例实现LRU存储机制
    LRUCache lru;
};


#endif