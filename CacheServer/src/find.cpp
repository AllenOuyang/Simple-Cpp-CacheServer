/*
 * @Author: your name
 * @Date: 2021-12-12 13:06:11
 * @LastEditTime: 2021-12-14 21:20:11
 * @LastEditors: your name
 * @Description: 
 * @FilePath: /CacheServer/src/find.cpp
 */
#include "find.hpp"

//findserver方法：输入index 输出节点的地址
std::string Find::findserver(std::string key)
{
    if (dist.size() != 120)
    {
        printf("分布数据有误:%ld\n", dist.size());
        exit(0);
    }
    std::string sha2 = picosha2::hash256_hex_string(key);
    int index = stoi(sha2.substr(59), 0, 16) % 120;
    char ip_s = dist[index];
    int ip_num = atoi(&ip_s);
    for (size_t i = 0; i < ip.size(); i++)
    {
        int n = atoi(ip[i].c_str());
        if (n == ip_num)
        {
            return (ip[i].substr(2));
        }
    }
    return "-1";
}
//renew方法：从ab分布中更新Find类
void Find::renew(std::string distribution)
{
    std::vector<std::string> vex;
    boost::split(vex, distribution, boost::is_any_of("+"));
    dist = vex[0];
    boost::split(ip, vex[1], boost::is_any_of(" "));
}