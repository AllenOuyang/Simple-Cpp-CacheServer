/*
 * @Author: your name
 * @Date: 2021-12-02 15:01:13
 * @LastEditTime: 2021-12-16 16:39:59
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: 
 * @FilePath: /CacheServer/include/TcpServer.hpp
 */
#ifndef _TCPSERVER_HPP
#define _TCPSERVER_HPP
#include "collect.hpp"
#include "TcpSocket.hpp"

class TcpServer
{
public:
    /**
     * @description: constructor without params
     * @param {*}
     * @return {*}
     */
    TcpServer(unsigned short port);

    /**
     * @description: destructor
     * @param {*}
     * @return {*}
     */    
    ~TcpServer();

    /**
     * @description: set listening to Client
     * @param {unsigned short} port
     * @return {*}
     */    
    int setListen();

    /**
     * @description: accept application of connection from clients
     * @param {sockaddr_in} *addr
     * @return {*}
     */    
    TcpSocket *acceptConn(struct sockaddr_in *addr = nullptr);

private:
    /*socket for listening Client*/
    int m_fd; 
    // port for client;
    int c_port;
};

#endif