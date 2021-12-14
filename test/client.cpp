/*************************************************************************
    > File Name: Client.cpp
    > Author: SongLee
    > E-mail: lisong.shine@qq.com
    > Created Time: 2016年05月05日 星期四 23时41分56秒
    > Personal Blog: http://songlee24.github.io/
 ************************************************************************/
#include<netinet/in.h>   // sockaddr_in
#include<sys/types.h>    // socket
#include<sys/socket.h>   // socket
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<iostream>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<cstring>
using namespace std;
#define BUFFER_SIZE 1024
 
enum Type {HEART, OTHER};
 
struct PACKET_HEAD
{
    Type type;
    int length;
};
 
void* send_heart(void* arg); 
 
class Client 
{
private:
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
    int fd;
public:
    Client(string ip, int port);
    ~Client();
    void Connect();
    void Run();
    friend void* send_heart(void* arg); 
};
 
Client::Client(string ip, int port)
{
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) == 0)
    {
        cout << "Server IP Address Error!";
        exit(1);
    }
    server_addr.sin_port = htons(port);
    server_addr_len = sizeof(server_addr);
    // create socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
        cout << "Create Socket Failed!";
        exit(1);
    }
}
 
Client::~Client()
{
    close(fd);
}
 
void Client::Connect()
{
    cout << "Connecting......" << endl;
    if(connect(fd, (struct sockaddr*)&server_addr, server_addr_len) < 0)
    {
        cout << "Can not Connect to Server IP!";
        exit(1);
    }
    cout << "Connect to Server successfully." << endl;
}
 
void Client::Run()
{
    pthread_t id;
    int ret = pthread_create(&id, NULL, send_heart, (void*)this);
    if(ret != 0)
    {
        cout << "Can not create thread!";
        exit(1);
    }
}
 
// thread function
void* send_heart(void* arg)
{
    cout << "The heartbeat sending thread started.\n";
    Client* c = (Client*)arg;
    int count = 0;  // 测试
    while(1) 
    {
        PACKET_HEAD head;
        head.type = HEART;
        head.length = 0;    
        send(c->fd, &head, sizeof(head), 0);
        sleep(3);     // 定时3秒
 		cout << "已发送" << endl;
        ++count;      // 测试：发送15次心跳包就停止发送
        if(count > 15)
            break;
    }
    cout << "cao" << endl;
}
 
int main()
{
    Client client("127.0.0.1", 15000);
    client.Connect();
    client.Run();
    while(1)
    {
        string msg;
        getline(cin, msg);
        if(msg == "exit")
            break;
        cout << "msg\n";
    }
    return 0;
}
