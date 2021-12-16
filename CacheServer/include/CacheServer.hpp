/*
 * @Author: your name
 * @Date: 2021-12-03 20:36:49
 * @LastEditTime: 2021-12-16 16:42:28
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: /CacheServer/include/CacheServer.hpp
 */
#ifndef _CACHESERVER_HPP
#define _CACHESERVER_HPP
#include "TcpServer.hpp"
#include "TcpSocket.hpp"
#include "lru.hpp"
#include "find.hpp"
#include <thread>

struct SockInfo
{
    TcpSocket *_tcp;
    struct sockaddr_in addr;
};

class CacheServer : public TcpServer
{
public:
    /**
     * @description: thread func for sending self-heartbeat
     * @param {*}
     * @return {*}
     */
    void sendHeart();

    /**
     * @description: thread func for listening new distribution got from master
     * @param {*}
     * @return {*}
     */    
    void recvDistr();

    /**
     * @description: thread func for recieving data from other servers
     * @param {void} *arg
     * @return {*}
     */
    void recvNewData();

    /**
     * @description: thread func for recieving new data from other servers (helper)
     * @param {void} *arg
     * @return {*}
     */
    void recieving(struct SockInfo *pinfo);

    /**
     * @description: thread func for interacting with clients
     * @param {void} *arg
     * @return {*}
     */
    void working(struct SockInfo *pinfo);

public:
    /**
     * @description: CacheServer's constructor with params
     * @param {int} _index
     * @param {int} _capacity
     * @return {*}
     */
    CacheServer(int _capacity, unsigned short port);

    /**
     * @description: fun for launch cacheserver
     * @param {*}
     * @return {*}
     */
    void run();

    /**
     * @description: set listening to master
     * @param {unsigned short} port
     * @return {*}
     */
    int setMasterListen();

    /**
     * @description: accept application of connection from Master
     * @param {sockaddr_in} *addr
     * @return {TcpSocket*} return a socket for communication
     */
    TcpSocket *acceptMasterConn(sockaddr_in *addr);

    /**
     * @description: set listening to other servers
     * @param {unsigned short} port
     * @return {*}
     */
    int setServerListen();

    /**
     * @description: accept application of connection from other servers
     * @param {sockaddr_in} *addr
     * @return {TcpSocket*} return a socket for communication
     */
    TcpSocket *acceptServerConn(sockaddr_in *addr);

    /**
     * @description: cache the key-value got from client
     * @param {string} _key
     * @param {string} _value
     * @return {*}
     */
    void put(std::string _key, std::string _value);

    /**
     * @description: inquire the corresponding value of a key
     * @param {string} _key
     * @return {string} value
     */
    std::string get(std::string _key);

    /**
     * @description: to update local chache's distribution
     * @param {*}
     * @return {*}
     */
    void update_distr();
    // 暂时不知道要不要写析构函数

private:
    // instantiate a LRUCache object to manage the cache
    LRUCache lru;
    // socket for listening Master
    int master_fd;
    // port for master
    unsigned short m_port;
    // socket for listening other servers
    int server_fd;
    //port for servers
    unsigned short s_port;
    // maintain a Find object to manage local kv distribution
    Find m_find;
};

#endif