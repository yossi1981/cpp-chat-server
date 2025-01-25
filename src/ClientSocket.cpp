#include "ClientSocket.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include "ServerSocket.h"
#include <exception>
#include <stdexcept>

#define BUF_SIZE 1024

ClientSocket::ClientSocket(int socketFd) : _socketFd(socketFd)
{
}

vector<uint8_t> ClientSocket::Read()
{
    uint8_t buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    ssize_t bytesRead = recv(_socketFd, buf, sizeof(buf) - 1, 0);

    if (bytesRead < 0)
    {
        throw runtime_error("could not read data from socket");
    }

    return vector<uint8_t>(reinterpret_cast<uint8_t *>(buf),
                        reinterpret_cast<uint8_t *>(buf) + sizeof(buf));
}

void ClientSocket::Send(const vector<uint8_t> &buf)
{
    size_t size = buf.size();
    send(_socketFd, buf.data(), size, 0);
}
