#include "MasterServer.hpp"

void *heart_handler(void *arg)
{
    std::cout << "The heartbeat checking thread started.\n";
    MasterServer *s = (MasterServer *)arg;
    while (1)
    {
        std::map<int, std::pair<std::string, int>>::iterator it = s->mmap.begin();
        for (; it != s->mmap.end();)
        {
            if (it->second.second == 5) // 3s*5没有收到心跳包，判定客户端掉线
            {
                std::cout << "The client " << it->second.first << " has be offline.\n"
                          << std::endl;

                int fd = it->first;
                close(fd); // 关闭该连接
                FD_CLR(fd, &s->master_set);
                if (fd == s->max_fd) // 需要更新max_fd;
                {
                    while (FD_ISSET(s->max_fd, &s->master_set) == false)
                        s->max_fd--;
                }

                s->mmap.erase(it++); // 从map中移除该记录
            }
            else if (it->second.second < 5 && it->second.second >= 0)
            {
                it->second.second += 1;
                ++it;
            }
            else
            {
                ++it;
            }
        }
        sleep(3); // 定时三秒
    }
}

void *recv_heart(void *arg)
{
    MasterServer *ms = (MasterServer *)arg;
    // 设置监听心跳包的端口
    ms->setListenForHeart(20001);
    
    pthread_t id; // 创建心跳检测线程
    int ret = pthread_create(&id, NULL, heart_handler, (void *)ms);
    if (ret != 0)
    {
        std::cout << "Can not create heart-beat checking thread."
                  << std::endl;
    }

    ms->max_fd = ms->listen_fd; // 初始化max_fd
    FD_ZERO(&ms->master_set);
    FD_SET(ms->listen_fd, &ms->master_set); // 添加监听fd

    while (1)
    {
        FD_ZERO(&ms->working_set);
        memcpy(&ms->working_set, &ms->master_set, sizeof(ms->master_set));

        ms->timeout.tv_sec = 30;
        ms->timeout.tv_usec = 0;

        int nums = select(ms->max_fd + 1, &ms->working_set, NULL, NULL, &ms->timeout);
        std::cout << nums << std::endl;
        if (nums < 0)
        {
            std::cout << "select() error!" << std::endl;
            exit(1);
        }

        if (nums == 0)
        {
            //cout << "select() is timeout!";
            continue;
        }

        if (FD_ISSET(ms->listen_fd, &ms->working_set))
        {
            std::cout << "hehe" << std::endl;
            ms->Accept(); // 有新的客户端请求
        }
        else
            ms->Recv(nums); // 接收客户端的消息
    }
}

void *working(void *arg)
{
    struct SockInfo *pinfo = static_cast<struct SockInfo *>(arg);
    // 连接建立成功, 打印客户端的IP和端口信息
    char ip[32];
    printf("客户端的IP: %s, 端口: %d\n",
           inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(pinfo->addr.sin_port));

    // 5. 通信
    while (1)
    {
        printf("接收数据: .....\n");
        std::string msg = pinfo->_tcp->recvMsg();
        if (!msg.empty())
        {
            std::cout << msg << std::endl
                      << std::endl
                      << std::endl;
            // 收到的msg是一个key或者key空格value
            // 如果是一个key，那么查询该key的分布并返回给Client
            // 如果是key空格value，那么告诉Client要把该key value对写在哪个CacheServer
            std::stringstream os;
            os << msg;
            std::string key, value;
            os >> key >> value;
            os.str("");
            if (value == "") // 如果只有一个key
            {
            }
            else // key和value都有
            {
            }
            // 把key value对存到LRUCache中
            // pinfo->_server->put(key, value);
        }
        else
        {
            break;
        }
    }
    delete pinfo->_tcp;
    delete pinfo;
    return nullptr;
}

MasterServer::MasterServer()
{
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        std::cout << "Create Socket Failed!" << std::endl;
        exit(1);
    }
}

void MasterServer::run()
{
    // MasterServer 需要两个端口，一个用于与Client通信，一个接收CacheServer的心跳包

    // 接收CacheServer的心跳包
    pthread_t tid;
    pthread_create(&tid, NULL, recv_heart, (void *)this);
    pthread_detach(tid);

    // 主线程与Client通信
    setListen(20000);
    // 阻塞并等待客户端连接
    while (1)
    {
        SockInfo *info = new SockInfo;
        TcpSocket *tcp = acceptConn(&info->addr);
        if (tcp == nullptr)
        {
            std::cout << "重试...." << std::endl;
            continue;
        }
        // 创建子线程
        pthread_t tid;
        info->_server = this;
        info->_tcp = tcp;

        pthread_create(&tid, NULL, working, info);
        pthread_detach(tid);
    }
}

// 继续完善

void MasterServer::Accept()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int new_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    std::cout << new_fd << "\n";/////
    if (new_fd < 0)
    {
        std::cout << "Server Accept Failed!" << std::endl;
        exit(1);
    }

    std::string ip(inet_ntoa(client_addr.sin_addr)); // 获取客户端IP

    std::cout << ip << " new connection was accepted.\n";

    mmap.insert(std::make_pair(new_fd, std::make_pair(ip, 0)));

    // 将新建立的连接的fd加入master_set
    FD_SET(new_fd, &master_set);
    if (new_fd > max_fd)
    {
        max_fd = new_fd;
    }
}

int MasterServer::setListenForHeart(unsigned short port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY; // 0 = 0.0.0.0
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    std::cout << "heihie" << std::endl;
    int ret = bind(listen_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }
    std::cout << "套接字绑定成功, ip: "
              << inet_ntoa(saddr.sin_addr)
              << ", port: " << port << std::endl;

    ret = listen(listen_fd, 128);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }
    std::cout << "设置监听成功..." << std::endl;

    return ret;
}

void MasterServer::Recv(int nums)
{
    for (int fd = 0; fd <= max_fd; ++fd)
    {
        if (FD_ISSET(fd, &working_set))
        {
            bool close_conn = false; // 标记当前连接是否断开了

            // TcpSocket tcp(fd);
            
            // std::string msg = tcp.recvMsg();
            HeartPackage HP;
            recv(fd, &HP, sizeof(HP), 0);
            std::cout << HP.CapaLru << std::endl;
            if (HP.CapaLru == 100)
            {
                mmap[fd].second = 0; // 每次收到心跳包，count置0
                std::cout << "Received heart-beat from client.\n"
                          << std::endl;
            }
            else
            {
                // 数据包，通过head.length确认数据包长度
            }

            if (close_conn) // 当前这个连接有问题，关闭它
            {
                close(fd);
                FD_CLR(fd, &master_set);
                if (fd == max_fd) // 需要更新max_fd;
                {
                    while (FD_ISSET(max_fd, &master_set) == false)
                        --max_fd;
                }
            }
        }
    }
}

MasterServer::~MasterServer()
{
    for (int fd = 0; fd <= max_fd; ++fd)
    {
        if (FD_ISSET(fd, &master_set))
        {
            close(fd);
        }
    }
}