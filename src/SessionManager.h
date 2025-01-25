#ifndef ___SessionManager___
#define ___SessionManager___

#include "defs.h"
#include <map>
#include <memory>
#include "ClientSocket.h"
#include <atomic>

using namespace std;
using spClientSocket = shared_ptr<ClientSocket>;

class SessionManager {
private:
    map<SessionToken, spClientSocket> _sessionTable;
    atomic<int> _maxSessions;

    SessionToken GenerateRandomToken();
public:
    SessionManager(int maxSessions);
    string CreateSession(spClientSocket clientSocket);
    void DestroySession(const string& sessionToken);
    spClientSocket GetClientSocket(const string& sessionToken);
};

#endif