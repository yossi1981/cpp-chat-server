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
        vector<uint8_t> data;
        bool _socket_valid = true;
        try
        {
            data = clientSocket->Read();
        }
        catch(const std::exception& e)
        {
            _socket_valid = false;
        }
        
        string msg(data.begin(), data.end());
        size_t end = msg.find_first_of("\n\r");
        if (end == std::string::npos) msg = "";
        msg =  msg.substr(0, end + 1);
        
        if(_socket_valid == false || msg == "exit") {
            _sessionManager.DestroySession(token);
            break;
        }
        stringstream sstr;
        sstr << "Message was recieved from client " << token << ": " << msg;
        E(sstr.str())
    }
}

void ChatServer::Listen() {
    _serverSocket = make_shared<ServerSocket>(_ipAddress, _port, _maxClients);
    _serverSocket->Accept([this](int clientSocket) {
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

void ChatServer::Shutdown() 
{
    _serverSocket->Shutdown();
    _sessionManager.Clear();

    
}
