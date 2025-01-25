#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include "ServerSocket.h"

ServerSocket::ServerSocket(const IpAddress &ipAddress,
                                       const Port &port,
                                       size_t maxClients) : _ipAddress(ipAddress),
                                                            _port(port),
                                                            _maxClients(maxClients)
{
}

void ServerSocket::Accept(ClientConnectedListener ClientConnectedListener)
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);

    int bindRes = bind(server_fd,
                       (struct sockaddr *)&address,
                       sizeof(address));
    if (bindRes < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, _maxClients) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    int clientSocket;
    while (true)
    {
        clientSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (clientSocket < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        ClientConnectedListener(clientSocket);
    }

    close(clientSocket);
    close(server_fd);
}
