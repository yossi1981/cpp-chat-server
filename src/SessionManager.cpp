#include "SessionManager.h"
#include <random>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

#define SESSION_TOKEN_LEN 8

SessionToken SessionManager::GenerateRandomToken()
{
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
_maxSessions(maxSessions)
{
}

string SessionManager::CreateSession(spClientSocket clientSocket)
{
    SessionToken token = GenerateRandomToken();
    _sessionTable[token] = clientSocket;
    return token;
}

void SessionManager::DestroySession(const string &sessionToken)
{
    _sessionTable.erase(sessionToken);
}

spClientSocket SessionManager::GetClientSocket(const string &sessionToken)
{
    return _sessionTable[sessionToken];
}
