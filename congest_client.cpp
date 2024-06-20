#include <iostream>
#include <unistd.h>
#include <QObject>
#include "defs.h"

using namespace std;

class Client: public QObject {
private:
    int sockfd;
    struct sockaddr_in serverAddr;
    void set_server_addr() {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serverAddr.sin_port = htons(SERVER_PORT);
    }

    void run_wsa_data() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("WSAStartup failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    void create_socket() {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            cerr << "Failed to create socket" << endl;
            exit(1);
        }
    }

public:

    Client(QObject* parent = nullptr): QObject{parent} {
        run_wsa_data();
        create_socket();
        set_server_addr();
    }

    void sendRequest() {
        char buffer[] = "Request";
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    }

    void receiveResponse() {
        char recvBuffer[1024];
        socklen_t serverAddrLen = sizeof(serverAddr);
        recvfrom(sockfd, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr*)&serverAddr, &serverAddrLen);
        cout << "Received response: " << recvBuffer << endl;
    }

    void simulate(int numRequests) {
        for (int i = 0; i < numRequests; i++) {
            sendRequest();
            cout << "Sent request " << i + 1 << endl;
            receiveResponse();
        }
    }
};
