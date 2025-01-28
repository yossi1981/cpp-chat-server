#include "SessionManager.h"
#include <random>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <exception>

using namespace std;

#define SESSION_TOKEN_LEN 8

SessionToken SessionManager::GenerateRandomToken() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0,SESSION_TOKEN_LEN - 1);
    stringstream strstream;
    for(int i = 0; i < SESSION_TOKEN_LEN -1 ; ++i) {
        strstream << hex << setw(1) << dis(gen);
    }
    return strstream.str();
}

SessionManager::SessionManager(int maxSessions) : 
_maxSessions(maxSessions) {
}

string SessionManager::CreateSession(spClientSocket clientSocket) {
    {
        std::lock_guard<std::mutex> lock(_sessionTableMtx);
        if (_sessionTable.size() == _maxSessions) {
            throw runtime_error("Can't create more sessions");
        }
    }
    
    SessionToken token = GenerateRandomToken();
    {

        std::lock_guard<std::mutex> lock(_sessionTableMtx);
        _sessionTable[token] = clientSocket;
    }
    return token;
}

void SessionManager::DestroySession(const string &sessionToken) {
    {
        std::lock_guard<std::mutex> lock(_sessionTableMtx);
        if (_sessionTable.size() == 0 || 
            _sessionTable.find(sessionToken) == _sessionTable.end()){
            throw runtime_error("Session token was not found");
        }
        _sessionTable.erase(sessionToken);
    }
}

spClientSocket SessionManager::GetClientSocket(const string &sessionToken) {
    {
        std::lock_guard<std::mutex> lock(_sessionTableMtx);
        if (_sessionTable.size() == 0 || 
            _sessionTable.find(sessionToken) == _sessionTable.end()){
            throw runtime_error("Session token was not found");
        }
        return _sessionTable[sessionToken];
    }
}

void SessionManager::Clear()
{
    std::lock_guard<std::mutex> lock(_sessionTableMtx);
    for (auto pair : _sessionTable) {
        pair.second->Close();
    }
    _sessionTable.clear();
}

