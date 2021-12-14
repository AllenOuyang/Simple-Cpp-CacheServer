#include "TcpServer.hpp"
#include "TcpSocket.hpp"
#include "MasterServer.hpp"

int main()
{
    MasterServer s;
    s.run();

    return 0;
}