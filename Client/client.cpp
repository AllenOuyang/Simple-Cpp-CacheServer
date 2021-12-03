#include "TcpSocket.hpp"
#include "TcpServer.hpp"
#include "client.hpp"
#include <fstream>


int main(int argc, char** argv)
{
    // 1.创建client对象
    client my_client;
    // 2.my_client打开本地数据分布缓存文件并且存到distrMap中
    my_client.read_info();
    // todo:如何实现均匀等长的key value对 并写入？？？
    
    // 5.查找对应的key是否在map中有对应的CacheServer信息
        // 如果有信息，那么根据该信息访问对应的CacheServer
        // 如果没有信息，那么访问Master拉取新的分布信息，并且更新distrmap

    // 6.







    // 以下是测试代码(只是为了测试TcpSocket类的通信功能，与课题无关)
    // 1. 创建通信的套接字
    // TcpSocket tcp;

    // // 2. 连接服务器IP port
    // int ret = tcp.connectToHost("127.0.0.1", 10000);
    // if (ret == -1)
    // {
    //     return -1;
    // }

    // // 3. 通信
	// std::fstream fd("data.txt", std::ios::in);
    // std::string tmp = "";
    // while (getline(fd, tmp))
    // {
    //     // 发送数据
    //     tcp.sendMsg(tmp);

	// 	std::cout << "send Msg: " << std::endl;
	// 	std::cout << tmp << std::endl << std::endl;
    //     tmp = "";

    //     // 接收数据
    //     sleep(3);
    // }

    // sleep(10);

    return 0;
}

