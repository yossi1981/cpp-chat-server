#ifndef CHAT_SERVER
#define CHAT_SERVER

#include "./defs.h"
#include <vector>
#include "ClientSocket.h"
#include "SessionManager.h"
#include "ServerSocket.h"


using namespace std;
 
class ChatServer
{
private:
    IpAddress _ipAddress;
    Port _port;
    size_t _maxClients;
    SessionManager _sessionManager;
    shared_ptr<ServerSocket> _serverSocket;

    SessionToken CreateSession(int clientSocketFD);
    void Interact(SessionToken token);
    void Listen();

public:
    ChatServer(const IpAddress &ipAddress,
                 const Port &port,
                 size_t maxClients);

    void Start();
    void Shutdown();
};

#endif