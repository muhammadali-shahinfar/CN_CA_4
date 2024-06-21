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

    void set_server_addr() {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(SERVER_PORT);
    }

    void run_wsa_data() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("WSAStartup failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    void bind_server_to_port() {
        if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            cerr << "Failed to bind socket" << endl;
            exit(1);
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
    Server(QObject* parent = nullptr): QObject{parent}  {
        cwnd = 1;
        ssthresh = MAX_WINDOW_SIZE;
        run_wsa_data();
        create_socket();
        set_server_addr();
        bind_server_to_port();
        cout << "Server listening on port " << SERVER_PORT << endl;
    }

    void sendPacket() {
        char recvBuffer[1024];
        socklen_t clientAddrLen = sizeof(clientAddr);

        // Wait for a request from the client
        recvfrom(sockfd, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
        cout << "Received request from client" << endl;

        // Send response
        char buffer[] = "Server data";
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, clientAddrLen);
        cout << "Sending packet, cwnd = " << cwnd << endl;
    }


    void receiveAck() {
        if (cwnd < ssthresh)
            cwnd++;
        else
            cwnd += 1.0 / cwnd;

        if (cwnd > MAX_WINDOW_SIZE)
            cwnd = MAX_WINDOW_SIZE;
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
