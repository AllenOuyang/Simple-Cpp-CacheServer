#ifndef _TCPSOCKET_HPP
#define _TCPSOCKET_HPP
#include "collect.hpp"

class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(int socket);
    ~TcpSocket();
    int connectToHost(std::string ip, unsigned short port);
    int sendMsg(std::string msg);
    std::string recvMsg();

private:
    int readn(char *buf, int size);
    int writen(const char *msg, int size);

private:
    int m_fd; // 通信的套接字
};

#endif