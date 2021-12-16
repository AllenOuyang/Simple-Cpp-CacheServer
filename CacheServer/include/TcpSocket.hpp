/*
 * @Author: your name
 * @Date: 2021-12-02 14:45:58
 * @LastEditTime: 2021-12-16 16:40:16
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: /CacheServer/include/TcpSocket.hpp
 */
#ifndef _TCPSOCKET_HPP
#define _TCPSOCKET_HPP
#include "collect.hpp"
#include <sstream>

class TcpSocket
{
public:
    /**
     * @description: constructor without param
     * @param {*}
     * @return {*}
     */
    TcpSocket();

    /**
     * @description:  constructor with param
     * @param {int} socket
     * @return {*}
     */    
    TcpSocket(int socket);

    /**
     * @description: destructor
     * @param {*}
     * @return {*}
     */    
    ~TcpSocket();

    /**
     * @description: connect to some address
     * @param {string} ip
     * @param {unsigned short} port
     * @return {*}0 on success , -1 for errors
     */    
    int connectToHost(std::string ip, unsigned short port);

    /**
     * @description: send message to the connected address 
     * @param {string} msg
     * @return {*}
     */    
    int sendMsg(std::string msg);

    /**
     * @description: recieve message from the connected address 
     * @param {*}
     * @return {*}string recieved
     */    
    std::string recvMsg();

private:
    /**
     * @description: 
     * @param {char} *buf
     * @param {int} size
     * @return {*}
     */
    int readn(char *buf, int size);

    /**
     * @description: 
     * @param {char} *msg
     * @param {int} size
     * @return {*}
     */    
    int writen(const char *msg, int size);

private:
//public:
    /*socket for communication*/
    int m_fd; 
};

#endif