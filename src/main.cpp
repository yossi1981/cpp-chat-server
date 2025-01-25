#include <iostream>
#include "ChatServer.h"

using namespace std;

void printHeader() {
    std::cout << "\n=============================================" << std::endl;
    std::cout << "            Chat Server Started              " << std::endl;
    std::cout << "=============================================" << std::endl;
}

void printConnectionInfo(const std::string& ipAddress, int port) {
    std::cout << "\nListening on: " << ipAddress << ":" << port << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
}

void printInstructions() {
    std::cout << "\nWaiting for a client connection..." << std::endl;
    std::cout << "Type 'exit' to quit the program." << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
}

void printExitMessage() {
    std::cout << "\n=============================================" << std::endl;
    std::cout << "              Program Exiting...             " << std::endl;
    std::cout << "=============================================" << std::endl;
}


int main() {
    
    string ipAddress = "127.0.0.1";
    int port = 8080;
    string input;
    ChatServer chatServer(ipAddress, port, 10);
    chatServer.Start();
    
    printHeader();
    printConnectionInfo(ipAddress, port);
    printInstructions();
    while (true){
        std::getline(std::cin, input);
        if (input == "exit") break;
    }
    printExitMessage();
    return 0;
}
