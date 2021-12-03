#ifndef _MASTERSERVER_HPP
#define _MASTERSERVER_HPP
#include "TcpServer.hpp"
#include "TcpSocket.hpp"

// 继承TcpServer
class MasterServer : public TcpServer
{
public:
    // 无参构造函数
    MasterServer();

    // 想想Master要实现哪些功能（提供哪些接口）...
};

#endif