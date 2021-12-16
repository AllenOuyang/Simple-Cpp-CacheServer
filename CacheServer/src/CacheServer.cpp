#include "CacheServer.hpp"

void CacheServer::sendHeart()
{
    std::cout << "The heartbeat sending thread started." << std::endl;
    /*create a socket to connect to Master*/
    TcpSocket tcp;
    /*connect to Master*/
    int ret = tcp.connectToHost("127.0.0.1", 20001);
    if (ret != -1)
        std::cout << "Connect to Master successfully!" << std::endl;
    /*Send a heartbeat package in every 3 seconds*/
    while (true)
    {
        tcp.sendMsg("alive");
        sleep(3);
    }
}

void CacheServer::recvDistr()
{
    setMasterListen();
    while (1)
    {
        SockInfo *info = new SockInfo;
        TcpSocket *tcp = acceptMasterConn(&info->addr);
        if (tcp == nullptr)
        {
            std::cout << "Retry to accept connection...." << std::endl;
            continue;
        }
        std::string distr_info = tcp->recvMsg();
        std::cout << "Recieved new distribution: " << distr_info << std::endl;
        m_find.renew(distr_info);
        update_distr();
    }
}

void CacheServer::recieving(struct SockInfo *pinfo)
{
    //struct SockInfo *pinfo = static_cast<struct SockInfo *>(arg);
    /*connect successfully , print the server's info of IP and Port*/
    char ip[32];
    std::cout << "Server's IP: " << inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip))
              << " Port: " << ntohs(pinfo->addr.sin_port) << std::endl;
    /*communicating*/
    while (1)
    {
        std::cout << "Reciving data from Server: ....." << std::endl;
        std::string key, value;
        std::string msg = pinfo->_tcp->recvMsg();
        std::cout << "Recieved data from " << inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip))
                  << " is : " << msg << std::endl;
        /*split msg into key and value*/
        std::stringstream os;
        os << msg;
        os >> key >> value;
        os.clear();
        /*save the newly got kv*/
        put(key, value);
    }
    delete pinfo->_tcp;
    delete pinfo;
}

void CacheServer::recvNewData()
{
    setServerListen();
    /*block and wait for the client to connect*/
    while (1)
    {
        struct SockInfo *info = new SockInfo;
        TcpSocket *tcp = acceptServerConn(&info->addr);
        if (tcp == nullptr)
        {
            std::cout << "Retry to accept connection...." << std::endl;
            continue;
        }
        /*create a child thread for recieving the data from the 
            just connected server*/
        std::thread recieve(&CacheServer::recieving, this, info);
        recieve.detach();
    }
}

void CacheServer::working(struct SockInfo *pinfo)
{
    /*connect successfully , print the client's info of IP and Port*/
    char ip[32];
    std::cout << "Client's IP: " << inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip))
              << " Port: " << ntohs(pinfo->addr.sin_port) << std::endl;

    /*communicating with client*/
    while (1)
    {
        std::cout << "Accept application from client: ....." << std::endl;
        std::string msg = pinfo->_tcp->recvMsg();
        std::cout << msg << std::endl;
        if (!msg.empty())
        {
            //std::cout << msg << std::endl << std::endl << std::endl;
            /*the gotten msg is a string contains a pair of key and value 
                that are splited by blank or a string contains only a key. 
                Then, use stringstream to split msg into key and value*/
            std::stringstream os;
            os << msg;
            std::string key, value;
            os >> key >> value;
            os.clear();
            /*if value isn't empty, then it is a 'Storage request'
                else if value is empty, that means it is a 'Query request'*/
            if (value != "")
            {
                if (m_find.findserver(key) == "127.0.0.1")
                {
                    put(key, value);
                    std::cout << "recived: "
                              << " [key] = " << key
                              << " [value] = " << value
                              << " Data saved successfully" << std::endl;
                    pinfo->_tcp->sendMsg("Yes!"); /*Yes represents kv was successfully saved*/
                }
                else
                {
                    std::cout << "recived: "
                              << " [key] = " << key
                              << " [value] = " << value
                              << " Application denied" << std::endl;
                    pinfo->_tcp->sendMsg("Denied!"); /*Denied represents kv should not be saved in this server*/
                }
            }
            else
            {
                if (m_find.findserver(key) == "127.0.0.1")
                {
                    std::string value = get(key);
                    if (value != "")
                    {
                        std::cout << " [key] = " << key
                                  << " get: "
                                  << " [value] = " << value
                                  << " value got successfully" << std::endl;
                        pinfo->_tcp->sendMsg(value); /*reply a value represents value was successfully found*/
                    }
                    else
                    {
                        std::cout << " [key] = " << key
                                  << " value not found" << std::endl;
                        pinfo->_tcp->sendMsg("Not Found!"); /*Not Found represents fail to find value*/
                    }
                }
                else
                {
                    std::cout << " [key] = " << key
                              << " [value] = " << value
                              << " Search denied" << std::endl;
                    pinfo->_tcp->sendMsg("Denied"); /*Denied represents value not belong to this server*/
                }
            }
        }
        else
        {
            break;
        }
    }
    delete pinfo->_tcp;
    delete pinfo;
}

CacheServer::CacheServer(int _capacity, unsigned short port) : TcpServer(port)
{
    lru.set_capacity(_capacity);
    master_fd = socket(AF_INET, SOCK_STREAM, 0);
    m_port = port + 1;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    s_port = port + 2;
}

void CacheServer::run()
{
    /*create a child thread for sending own heartbeat package to master*/
    // std::thread send_Heart(&CacheServer::sendHeart, this);
    // send_Heart.detach();

    /*create a child thread for recieving new distribution of data from master*/
    std::thread recv_distr(&CacheServer::recvDistr, this);
    recv_distr.detach();

    /*create a child thread for recieving new Data from other cacheservers*/
    std::thread recvData(&CacheServer::recvNewData, this);
    recvData.detach();

    /*set listening port for interacting with clients*/
    setListen();
    /*block and wait for the client to connect*/
    while (1)
    {
        struct SockInfo *info = new SockInfo;
        TcpSocket *tcp = acceptConn(&info->addr);
        if (tcp == nullptr)
        {
            std::cout << "Retry to accept connection...." << std::endl;
            continue;
        }
        info->_tcp = tcp;
        /*create a child thread for interacting with client*/
        std::thread work(&CacheServer::working, this, info);
        work.detach();
    }
}

int CacheServer::setMasterListen()
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(m_port);
    saddr.sin_addr.s_addr = INADDR_ANY; // 0 = 0.0.0.0
    int ret = bind(master_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind Master");
        return -1;
    }
    std::cout << "Socket for Master was bound successfully, ip: "
              << inet_ntoa(saddr.sin_addr)
              << ", port: " << m_port << std::endl;

    ret = listen(master_fd, 128);
    if (ret == -1)
    {
        perror("listen Master");
        return -1;
    }
    std::cout << "Set listening for Master successfully..." << std::endl;

    return ret;
}

TcpSocket *CacheServer::acceptMasterConn(sockaddr_in *addr)
{
    if (addr == NULL)
    {
        return nullptr;
    }

    socklen_t addrlen = sizeof(struct sockaddr_in);
    int cfd = accept(master_fd, (struct sockaddr *)addr, &addrlen);
    if (cfd == -1)
    {
        perror("accept Master");
        return nullptr;
    }
    printf("successfully connect to Master...\n");
    return new TcpSocket(cfd);
}

int CacheServer::setServerListen()
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(s_port);
    saddr.sin_addr.s_addr = INADDR_ANY; // 0 = 0.0.0.0
    int ret = bind(server_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind Server");
        return -1;
    }
    std::cout << "Socket for Server was bound successfully, ip: "
              << inet_ntoa(saddr.sin_addr)
              << ", port: " << s_port << std::endl;

    ret = listen(server_fd, 128);
    if (ret == -1)
    {
        perror("listen Server");
        return -1;
    }
    std::cout << "Set listening for Server successfully..." << std::endl;

    return ret;
}

TcpSocket *CacheServer::acceptServerConn(sockaddr_in *addr)
{
    if (addr == NULL)
    {
        return nullptr;
    }

    socklen_t addrlen = sizeof(struct sockaddr_in);
    int cfd = accept(server_fd, (struct sockaddr *)addr, &addrlen);
    if (cfd == -1)
    {
        perror("accept Server");
        return nullptr;
    }
    printf("successfully connect to Server...\n");
    return new TcpSocket(cfd);
}

void CacheServer::put(std::string _key, std::string _value)
{
    lru.put(_key, _value);
}

std::string CacheServer::get(std::string _key)
{
    return lru.get(_key);
}

void CacheServer::update_distr()
{
    /*Traverse every single key and see if it still belong to this server
        under the new distribution*/
    for (auto e : lru.cache)
    {
        std::string ip = m_find.findserver(e.first);
        if (ip != "127.0.0.1") /*if it do not belong to, then send it to where it should be*/
        {
            TcpSocket tcp;
            tcp.connectToHost(ip, 6666); //TODO:this is just test port, wait to modify
            std::string kv = e.second->key + " " + e.second->value;
            tcp.sendMsg(kv);
            std::cout << "have sent: " << kv << " to its new server: " << ip << std::endl;
            /*remove this node*/
            lru.removeNode(e.second);
        }
        /*if it belongs to, do nothing*/
    }
}
