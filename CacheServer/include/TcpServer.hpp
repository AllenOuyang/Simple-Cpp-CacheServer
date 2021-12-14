/*
 * @Author: your name
 * @Date: 2021-12-02 15:01:13
 * @LastEditTime: 2021-12-15 01:49:39
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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
    TcpServer();

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
    int setListen(unsigned short port);

    /**
     * @description: accept application of connection from clients
     * @param {sockaddr_in} *addr
     * @return {*}
     */    
    TcpSocket *acceptConn(struct sockaddr_in *addr = nullptr);

private:
    /*socket for listening Client*/
    int m_fd; 
};

#endif