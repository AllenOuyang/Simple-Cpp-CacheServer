#ifndef _CACHESERVER_HPP
#define _CACHESERVER_HPP
#include "TcpServer.hpp"
#include "TcpSocket.hpp"
#include "lru.hpp"

class CacheServer : public TcpServer
{
public:
    // 无参构造函数
    CacheServer();

    // 暂时不知道要不要写析构函数

public:
    // 利用LRUCache实例实现LRU存储机制
    LRUCache lru;
};


#endif