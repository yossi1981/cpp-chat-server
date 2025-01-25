#include "ChatServer.h"
#include "ServerSocket.h"
#include <thread>
#include <sstream>
#include <iostream>

using namespace std;

ChatServer::ChatServer(
    const IpAddress &ipAddress,
    const Port &port,
    size_t maxClients) : _ipAddress(ipAddress),
                         _port(port),
                         _maxClients(maxClients),
                         _sessionManager(maxClients)
{
}

SessionToken ChatServer::CreateSession(int clientSocketFD)
{
    spClientSocket clientSocket =  make_shared<ClientSocket>(clientSocketFD);
    SessionToken token = _sessionManager.CreateSession(clientSocket);
    stringstream sstr;
    sstr << "Session created. Your token is " << token << endl;
    string msg = sstr.str();
    vector<uint8_t> codedMsg(msg.begin(), msg.end());
    clientSocket->Send(codedMsg) ;
    return token;
}

void ChatServer::Interact(SessionToken token)
{
    spClientSocket clientSocket = _sessionManager.GetClientSocket(token);
    while(true) {
        vector<uint8_t> data = clientSocket->Read();
        string msg(data.begin(), data.end());
        
        if(msg == "exit") {
            //disconnect
            break;
        }
        stringstream sstr;
        sstr << "Message was recieve from client " << token << ": " << msg;
        E(sstr.str())
    }
}

void ChatServer::Listen() {
    ServerSocket serverSocket(_ipAddress, _port, _maxClients);
    serverSocket.Accept([this](int clientSocket) {
        thread clientThread([this, clientSocket]() {
            SessionToken token = CreateSession(clientSocket);
            Interact(token);       
        });
        clientThread.detach();
    });

}

void ChatServer::Start()
{
    thread serverThread([this](){
        Listen();
    });
    serverThread.detach();
}
