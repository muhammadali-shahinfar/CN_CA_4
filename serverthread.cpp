#include "serverthread.h"
#include <iostream>
#include <QRandomGenerator>
#include <string>

serverThread::serverThread(QObject *parent)
    : QThread{parent}
{}
void serverThread::run(){

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }
    SOCKET init_socket;
    if((init_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        std::cout <<(WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8025);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(init_socket, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));
    listen(init_socket, 5);
    this->server_socket = accept(init_socket, nullptr, nullptr);
    recv_syn();
    send_syn_ack();
    recv_ack();
    closesocket(init_socket);
    WSACleanup();
}


void serverThread::recv_syn(){

    char buffer[1024] = {0}; // maybe has bug
    recv(this->server_socket,buffer, sizeof(buffer), 0);
    std::cout << "client syn:" << buffer << std::endl;
    char* end;
    this->client_syn = strtol(buffer,&end,10);
    std::cout << "client syn:" << this->client_syn << std::endl;
}
void serverThread::send_syn_ack(){

    int random_value = QRandomGenerator::global()->generate();
    std::string message = std::to_string((random_value));
    message += " " + std::to_string(this->client_syn + 1);
    send(this->server_socket,message.c_str(),strlen(message.c_str()),0);
    std::cout << "server sin and ack:" << message << std::endl;
    this->syn = random_value;
}
void serverThread::recv_ack(){

    char buffer[1024] = {0}; // maybe has bug
    recv(this->server_socket,buffer, sizeof(buffer), 0);
    char* end;
    int client_ack= strtol(buffer,&end,10);
    std::cout << "client_ack:" << client_ack << std::endl;
    if(client_ack == this->syn + 1){
        std::cout << "ack received" << std::endl;
    }
    else send_syn_ack();
}
