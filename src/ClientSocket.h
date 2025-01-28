#ifndef CHAT_CLIENT_SOCKET
#define CHAT_CLIENT_SOCKET

#include "./defs.h"
#include <vector>
#include <cstdint>

using namespace std;

class ClientSocket
{
private:
    int _socketFd;

public:
    ClientSocket(int socketFd);
    ~ClientSocket();
    vector<uint8_t> Read();
    void Send(const vector<uint8_t> &buf);
    void Close();
};

#endif