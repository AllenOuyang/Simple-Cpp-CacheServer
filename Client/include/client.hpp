#ifndef _CLIENT_HPP
#define _CLIENT_HPP
#include "TcpSocket.hpp"
#include "collect.hpp"
#include <unordered_map>

// 本地的数据分布信息存下面这个文件名的文件中
#define DISTRFILE "distributionfile.txt"

class client
{
public:
    // 无参构造函数
    client();
    // 打开本地文件文件DISTRFILE读取分布信息,并读取到map中储存
    void read_info();
    // 读取key所对应的数据
    std::string get(std::string key);
    // 写入key value对
    void put(std::string key, std::string value);

    //todo:后续的方法（接口）继续补充...
    //析构函数暂时还不知道要不要写

public:
    TcpSocket m_tcp;
    //<string, string>第一个是key， 第二个是对应的CacheServer
    std::unordered_map<std::string, std::string> distrMap;
};

#endif