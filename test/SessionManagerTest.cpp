#include "../src/SessionManager.h"
#include "../src/ClientSocket.h"
#include <gtest/gtest.h>
#include <memory>
#include <string>

class MockClientSocket : public ClientSocket {
public:
    MockClientSocket() : ClientSocket(1) {}
};

class SessionManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        sessionManager = std::make_unique<SessionManager>(10);
    }

    void TearDown() override {
        sessionManager.reset();
    }

    std::unique_ptr<SessionManager> sessionManager;
};

TEST_F(SessionManagerTest, CreateSession) {
    spClientSocket clientSocket = std::make_shared<MockClientSocket>();
    std::string sessionToken = sessionManager->CreateSession(clientSocket);
    ASSERT_FALSE(sessionToken.empty());
    spClientSocket retrievedSocket = sessionManager->GetClientSocket(sessionToken);
    ASSERT_EQ(clientSocket, retrievedSocket);
}

TEST_F(SessionManagerTest, GetClientSocketWithInvalidToken) {
    std::string invalidToken = "invalid_token";
    ASSERT_THROW(spClientSocket retrievedSocket = sessionManager->GetClientSocket(invalidToken), runtime_error);
}

TEST_F(SessionManagerTest, DestroySession) {
    spClientSocket clientSocket = std::make_shared<MockClientSocket>();
    std::string sessionToken = sessionManager->CreateSession(clientSocket);
    sessionManager->DestroySession(sessionToken);
    ASSERT_THROW(spClientSocket retrievedSocket = sessionManager->GetClientSocket(sessionToken), runtime_error);
}

TEST_F(SessionManagerTest, ExceedMaxSessions) {
    for (int i = 0; i < 10; ++i) {
        std::shared_ptr<MockClientSocket> clientSocket = std::make_shared<MockClientSocket>();
        sessionManager->CreateSession(clientSocket);
    }

    std::shared_ptr<MockClientSocket> extraClientSocket = std::make_shared<MockClientSocket>();
    ASSERT_THROW(std::string sessionToken = sessionManager->CreateSession(extraClientSocket), runtime_error);
}