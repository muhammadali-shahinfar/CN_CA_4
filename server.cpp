#include "server.h"
#include "serverthread.h"

#include <Qthread>

sockaddr_in Server::create_sockaddr_in(int port) {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port );
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    return serverAddress;
}

SOCKET Server::connection_detail(int port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }
    SOCKET init_socket;
    if((init_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        exit(EXIT_FAILURE);
    }
    sockaddr_in serverAddress = create_sockaddr_in(port);
    bind(init_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    return init_socket;
}

void Server::new_connection()
{
    SOCKET init_socket = connection_detail(65258);
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



Server::Server(QObject *parent) : QObject{parent}{

}
