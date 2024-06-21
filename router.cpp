#include "router.h"
#include <iostream>

router::router(QObject *parent)
    : Server{parent}
{

    slidingWindowServerThread* sender_thread = new slidingWindowServerThread(1, 8);
    slidingWindowServerThread* receiver_thread = new slidingWindowServerThread(2, 8);

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }
    SOCKET host_A_socket
        = socket(AF_INET, SOCK_DGRAM, 0); // Use IPPROTO_UDP for UDP
    if (host_A_socket == INVALID_SOCKET) {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    SOCKET host_B_socket
        = socket(AF_INET, SOCK_DGRAM, 0); // Use IPPROTO_UDP for UDP
    if (host_B_socket == INVALID_SOCKET) {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_A_address = this->create_sockaddr_in(8085);
    struct sockaddr_in server_B_address = this->create_sockaddr_in(8086);
    if (bind(host_A_socket, (const struct sockaddr*)&server_A_address, sizeof(server_A_address)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        c
            sesocket(host_A_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    if (bind(host_B_socket, (const struct sockaddr*)&server_B_address, sizeof(server_B_address)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(host_B_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    int len_A;
    struct sockaddr_in client_A_address;
    len_A = sizeof(client_A_address);
    int len_B;
    struct sockaddr_in client_B_address;
    len_B = sizeof(client_A_address);
    char buffer_A[1700];
    char buffer_B[1700];
    int n = recvfrom(host_A_socket, (char*)buffer_A, 1700, 0, (struct sockaddr *)&client_A_address, &len_A );
    if (n == SOCKET_ERROR) {
        printf("recvfrom failed with error: %d\n", WSAGetLastError());
    }
    n = recvfrom(host_B_socket, (char*)buffer_B, 1700, 0, (struct sockaddr *)&client_B_address, &len_B );
    sendto(host_B_socket, buffer_A, sizeof(buffer_A), 0, (struct sockaddr*)&server_B_address, sizeof(server_B_address));
    if (n == SOCKET_ERROR) {
        printf("recvfrom failed with error: %d\n", WSAGetLastError());
    }
    sendto(host_A_socket, buffer_B, sizeof(buffer_B), 0, (struct sockaddr*)&server_A_address, sizeof(server_A_address));

    while(true){
        for(int i=0;i<8;i++){
            int n = recvfrom(host_A_socket, (char*)buffer_A, 1700, 0, (struct sockaddr *)&client_A_address, &len_A );
            if (n == SOCKET_ERROR) {
                printf("recvfrom failed with error: %d\n", WSAGetLastError());
            }
            std::cout << buffer_A <<std::endl;
            sendto(host_B_socket, buffer_A, sizeof(buffer_A), 0, (struct sockaddr*)&server_B_address, sizeof(server_B_address));
        }
        for(int i=0;i<8;i++){
            n = recvfrom(host_B_socket, (char*)buffer_B, 1700, 0, (struct sockaddr *)&client_B_address, &len_B );
            if (n == SOCKET_ERROR) {
                printf("recvfrom failed with error: %d\n", WSAGetLastError());
            }
            sendto(host_A_socket, buffer_B, sizeof(buffer_A), 0, (struct sockaddr*)&server_A_address, sizeof(server_A_address));
        }


    }
    closesocket(host_A_socket);
    closesocket(host_B_socket);
    WSACleanup();
}

