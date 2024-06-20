#include "server.h"
#include "serverthread.h"

#include <Qthread>

sockaddr_in Server::create_sockaddr_in() {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8025 );
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    return serverAddress;
}

Server::Server(QObject *parent) : QObject{parent}{

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }
    SOCKET init_socket;
    if((init_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        exit(EXIT_FAILURE);
    }
    sockaddr_in serverAddress = create_sockaddr_in();
    bind(init_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(init_socket, 5);
    int i=1;

    while(true){
        SOCKET new_socket = accept(init_socket,nullptr,nullptr);
        serverThread* thread = new serverThread(i+1,this);
        thread->connection_income(new_socket);
        thread->start();
        i++;
        // thread->run();
    }
}
