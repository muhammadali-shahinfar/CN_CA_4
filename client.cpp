#include "client.h"
#include <QRandomGenerator>
#include <iostream>
client::client() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }

    this->client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8025);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // sending connection request
    connect(client_socket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));

    send_SYN();
    recv_ack();
    send_ack();

}

void client::send_SYN(){
    int random_value = QRandomGenerator::global()->generate();
    std::string message = std::to_string((random_value));
    send(this->client_socket,message.c_str(),strlen(message.c_str()),0);
    this->sync_value = random_value;
    std::cout << "client SyN:" << random_value << std::endl;
}
void client::recv_ack(){
    char buffer[1024] = {0}; // maybe has bug
    recv(this->client_socket,buffer, sizeof(buffer), 0);
    std::cout << "server ack buffer:" << buffer << std::endl;
    char* end;
    int ack, server_syn;
    server_syn = strtol(buffer,&end,10);
    ack = strtol(end,&end,10);

    if(this->sync_value +1 == ack){
        this->server_sync_value = server_syn;
    }
    else
        send_SYN();
}

void client::send_ack(){
    int ack = this->server_sync_value + 1;
    std::string message = std::to_string((ack));
    send(this->client_socket, message.c_str(),strlen(message.c_str()), 0);
    std::cout << message << std::endl;
}
