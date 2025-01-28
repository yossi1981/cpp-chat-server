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
    int _server_socket_fd;
    bool _shut_down;

    void _releaseUnderlyingSocket(); 
    
public:
    using ClientConnectedListener = function<void(int)>;
    ServerSocket(const IpAddress &ipAddress,
                 const Port &port,
                 size_t maxClients);
    ~ServerSocket();

    void Accept(ClientConnectedListener ClientConnectedListener);
    void Shutdown();
};

#endif