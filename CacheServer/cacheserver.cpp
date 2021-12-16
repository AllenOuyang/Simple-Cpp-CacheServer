/*
 * @Author: your name
 * @Date: 2021-12-02 15:32:22
 * @LastEditTime: 2021-12-16 16:39:15
 * @LastEditors: your name
 * @Description:
 * @FilePath: /CacheServer/cacheserver.cpp
 */
#include "TcpSocket.hpp"
#include "CacheServer.hpp"

int main()
{
    /*create a instance of CacheServer with first parms = 100 (set capacity of LRU)
        second param = 6666 (the port number for listening clients, and 6666+1 for master
        6666+2 for servers)*/
    CacheServer m_Cache(100, 6666);
    /*launch cache server*/
    m_Cache.run();
    return 0;
}