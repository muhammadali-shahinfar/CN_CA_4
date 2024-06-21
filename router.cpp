#include "router.h"
#include <iostream>

router::router(QObject *parent)
    : Server{parent}
{

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }
    SOCKET host_A = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Use IPPROTO_UDP for UDP
    if (host_A == INVALID_SOCKET) {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address = this->create_sockaddr_in(8085);
    if (bind(host_A, (const struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(host_A);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    int len;
    struct sockaddr_in client_address;
    len = sizeof(client_address);
    char buffer[1700];
    int i =1;
    while(true){
    int n = recvfrom(host_A, (char*)buffer, 1700, 0, (struct sockaddr *)&client_address, &len );
    if (n == SOCKET_ERROR) {
        printf("recvfrom failed with error: %d\n", WSAGetLastError());
    }
    slidingWindowServerThread* thread = new slidingWindowServerThread(i);
    if(i==2)
        connect(&threads[0],&slidingWindowServerThread::message_received, &thread,&slidingWindowServerThread::send_message);
    thread->connection_income(client_address);
    thread->run();
    i++;
    }
    closesocket(host_A);
    WSACleanup();
}

void router::read_ready_A(){
}

