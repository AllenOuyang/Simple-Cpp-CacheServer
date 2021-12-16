/*
 * @Author: your name
 * @Date: 2021-12-02 15:01:27
 * @LastEditTime: 2021-12-16 16:39:35
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: /CacheServer/src/TcpServer.cpp
 */
#include "TcpServer.hpp"


// 构造函数，创建用于监听的套接字，ipv4流式传输Tcp协议
TcpServer::TcpServer(unsigned short port)
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    c_port = port;
}

// 析构函数，关闭用于监听的套接字
TcpServer::~TcpServer()
{
    close(m_fd);
}

int TcpServer::setListen()
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(c_port);
    saddr.sin_addr.s_addr = INADDR_ANY; // 0 = 0.0.0.0
    int ret = bind(m_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind Client");
        return -1;
    }
    std::cout << "Socket for Client was bound successfully, ip: "
              << inet_ntoa(saddr.sin_addr)
              << ", port: " << c_port << std::endl;

    ret = listen(m_fd, 128);
    if (ret == -1)
    {
        perror("listen Client");
        return -1;
    }
    std::cout << "Set listening for Client successfully..." << std::endl;

    return ret;
}

TcpSocket *TcpServer::acceptConn(sockaddr_in *addr)
{
    if (addr == NULL)
    {
        return nullptr;
    }

    socklen_t addrlen = sizeof(struct sockaddr_in);
    int cfd = accept(m_fd, (struct sockaddr *)addr, &addrlen);
    if (cfd == -1)
    {
        perror("accept Client");
        return nullptr;
    }
    printf("Successfully connect to Client...\n");
    return new TcpSocket(cfd);
}
