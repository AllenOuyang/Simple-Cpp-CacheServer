#ifndef _MASTERSERVER_HPP
#define _MASTERSERVER_HPP
#include "TcpServer.hpp"
#include "TcpSocket.hpp"
#include <sstream>
#include <unordered_map>
#include <map>
#include <utility>
#include <vector>


struct HeartPackage
{
    int index;
    int sizeLru;
    int CapaLru;
};

// 继承TcpServer
class MasterServer : public TcpServer
{
public:
    // 用于处理CacheServer心跳包的线程函数
    friend void *heart_handler(void *arg);

    // 用于接受CacheServer心跳包的线程函数
    friend void *recv_heart(void *arg);

    // 用于与client通信的线程
    friend void *working(void *arg);

    // 无参构造函数
    MasterServer();

    void run();

    // 想想Master要实现哪些功能（提供哪些接口）...

    void Accept();

    int setListenForHeart(unsigned short port);

    void Recv(int nums);

    ~MasterServer();

private:
    // 维护一个map来存储key的分布信息，string是key，vector存了CacheServer编号0，1，2
    std::unordered_map<std::string, std::vector<int>> distrmap;

    int listen_fd;      // 监听的fd
    int max_fd;         // 最大的fd
    fd_set master_set;  // 所有fd集合，包括监听fd和客户端fd
    fd_set working_set; // 工作集合
    struct timeval timeout;
    std::map<int, std::pair<std::string, int>> mmap; // 记录连接的客户端fd--><ip, count>
};

struct SockInfo
{
    MasterServer *_server;
    TcpSocket *_tcp;
    struct sockaddr_in addr;
};

#endif