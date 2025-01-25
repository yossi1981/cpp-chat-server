#ifndef CHAT_SERVER_SOCKET
#define CHAT_SERVER_SOCKET
#include "./defs.h"
#include <vector>
#include <functional>

using namespace std;

class ServerSocket
{
private:
    IpAddress _ipAddress;
    Port _port;
    size_t _maxClients;
public:
    using ClientConnectedListener = function<void(int)>;
    ServerSocket(const IpAddress &ipAddress,
                 const Port &port,
                 size_t maxClients);

    void Accept(ClientConnectedListener ClientConnectedListener);
};

#endif