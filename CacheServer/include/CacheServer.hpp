/*
 * @Author: your name
 * @Date: 2021-12-03 20:36:49
 * @LastEditTime: 2021-12-15 01:46:35
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

class CacheServer : public TcpServer
{
    /**
     * @description: thread func for sending self-heartbeat
     * @param {void} *arg
     * @return {*}
     */
    friend void *send_heart(void *arg);

    /**
     * @description: thread func for recieving data from other servers
     * @param {void} *arg
     * @return {*}
     */
    friend void *recvNewData(void *arg);

    /**
     * @description: thread func for listening new distribution got from master
     * @param {void} *arg
     * @return {*}
     */
    friend void *recv_distr(void *arg);

    /**
     * @description: thread func for recieving new data from other servers (helper)
     * @param {void} *arg
     * @return {*}
     */
    friend void *recieving(void *arg);

    /**
     * @description: thread func for interacting with clients
     * @param {void} *arg
     * @return {*}
     */
    friend void *working(void *arg);

public:
    /**
     * @description: CacheServer's constructor with params
     * @param {int} _index
     * @param {int} _capacity
     * @return {*}
     */
    CacheServer(int _index, int _capacity);

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
    int setMasterListen(unsigned short port);

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
    int setServerListen(unsigned short port);

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
    // the index of this CacheServer
    int index;
    // socket for listening Master
    int master_fd;
    // socket for listening other servers
    int server_fd;
    // maintain a Find object to manage local kv distribution
    Find m_find;
};

struct SockInfo
{
    CacheServer *_server;
    TcpSocket *_tcp;
    struct sockaddr_in addr;
};

#endif