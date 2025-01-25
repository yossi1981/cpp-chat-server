#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "defs.h"
#include <map>
#include <memory>
#include "ClientSocket.h"
#include <atomic>
#include <mutex>

using namespace std;

using spClientSocket = shared_ptr<ClientSocket>;

class SessionManager {
private:
    map<SessionToken, spClientSocket> _sessionTable;
    mutex _sessionTableMtx;
    atomic<int> _maxSessions;
    SessionToken GenerateRandomToken();
public:
    explicit SessionManager(int maxSessions);
    string CreateSession(spClientSocket clientSocket);
    void DestroySession(const string& sessionToken);
    spClientSocket GetClientSocket(const string& sessionToken);
};

#endif