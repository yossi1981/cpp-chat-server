#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include "ServerSocket.h"
#include <stdexcept>

ServerSocket::ServerSocket(const IpAddress &ipAddress,
                           const Port &port,
                           size_t maxClients) : _ipAddress(ipAddress),
                                                _port(port),
                                                _maxClients(maxClients),
                                                _server_socket_fd(0),
                                                _shut_down(false)
{
}

ServerSocket::~ServerSocket()
{
    _releaseUnderlyingSocket();
}

void ServerSocket::_releaseUnderlyingSocket()
{
    if (_server_socket_fd != 0)
    {
        close(_server_socket_fd);
        E("Server socket file descriptor has been closed");
    }
}

void ServerSocket::Shutdown()
{
    _shut_down = true;
    if (shutdown(_server_socket_fd, SHUT_RD) < 0)
    {
        throw std::runtime_error("Shutdown failed");
    }
    _releaseUnderlyingSocket();
}

void ServerSocket::Accept(ClientConnectedListener ClientConnectedListener)
{
    if (_server_socket_fd != 0)
    {
        throw std::runtime_error("Socket file descriptor is not 0");
    }

    if ((_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        throw std::runtime_error("Socket creation failed");
    }

    int opt = 1;
    if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        throw std::runtime_error("setsockopt SO_REUSEADDR failed");
    }

    if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        throw std::runtime_error("setsockopt SO_REUSEPORT failed");
    }

    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);

    int bindRes = bind(_server_socket_fd,
                       (struct sockaddr *)&address,
                       sizeof(address));
    if (bindRes < 0)
    {
        throw std::runtime_error("Socket binding failed");
    }

    if (listen(_server_socket_fd, _maxClients) < 0)
    {
        throw std::runtime_error("Listening to a socket failed");
    }

    int clientSocket;
    while (true)
    {
        clientSocket = accept(_server_socket_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (_shut_down == true)
        {
            break;
        }
        if (clientSocket < 0)
        {
            throw std::runtime_error("Listening to a socket failed");
        }
        ClientConnectedListener(clientSocket);
    }
}
