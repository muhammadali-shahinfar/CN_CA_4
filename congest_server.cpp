#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <QObject>
#include "defs.h"
using namespace std;


class Server: public QObject {
private:
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    int cwnd;
    int ssthresh;

public:
    Server(QObject* parent = nullptr): QObject{parent}  {
        cwnd = 1;
        ssthresh = MAX_WINDOW_SIZE;

            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            cerr << "Failed to create socket" << endl;
            exit(1);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(SERVER_PORT);

        if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            cerr << "Failed to bind socket" << endl;
            exit(1);
        }

        cout << "Server listening on port " << SERVER_PORT << endl;
    }

    void sendPacket() {
        char buffer[] = "Dummy data";
        socklen_t clientAddrLen = sizeof(clientAddr);
        recvfrom(sockfd, nullptr, 0, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
        cout << "Sending packet, cwnd = " << cwnd << endl;
    }

    void receiveAck() {
        if (cwnd < ssthresh) {
            cwnd++;
        } else {
            cwnd += 1.0 / cwnd;
        }

        if (cwnd > MAX_WINDOW_SIZE) {
            cwnd = MAX_WINDOW_SIZE;
        }
    }

    void packetLoss() {
        ssthresh = cwnd / 2;
        cwnd = 1;
        cout << "Packet loss detected, ssthresh = " << ssthresh << ", cwnd = " << cwnd << endl;
    }

    void simulate(int numPackets) {
        for (int i = 0; i < numPackets; i++) {
            sendPacket();
            receiveAck();

            if (rand() % 10 == 0) {
                packetLoss();
            }
        }
    }
};
