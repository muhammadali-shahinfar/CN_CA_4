#include "serverthread.h"
#include <iostream>
#include <QRandomGenerator>
#include <string>

serverThread::serverThread(int n,QObject *parent) : QThread{parent} {
    this->client_num = n;
}

sockaddr_in serverThread::create_sockaddr_in() {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8025 + this->client_num);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    return serverAddress;
}

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
    sockaddr_in serverAddress = create_sockaddr_in();
    bind(init_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(init_socket, 5);
    this->server_socket = accept(init_socket, nullptr, nullptr);
    start_messaging();
}

void serverThread::start_messaging() {
    recv_syn();
    send_syn_ack();
    recv_ack();
    recv_message();
}

void serverThread::recv_syn() {
    char buffer[1024] = {0}; // maybe has bug
    recv(this->server_socket,buffer, sizeof(buffer), 0);
    std::cout << "client syn:" << buffer << std::endl;
    char* end;
    this->client_syn = strtol(buffer,&end,10);
    std::cout << "client syn:" << this->client_syn << std::endl;
}

void serverThread::send_syn_ack() {
    int random_value = QRandomGenerator::global()->generate();
    std::string message = std::to_string((random_value));
    message += " " + std::to_string(this->client_syn + 1);
    send(this->server_socket,message.c_str(),strlen(message.c_str()),0);
    std::cout << "server sin and ack:" << message << std::endl;
    this->syn = random_value;
    this->client_syn++;
}

void serverThread::recv_ack(){

    char buffer[1024] = {0}; // maybe has bug
    recv(this->server_socket,buffer, sizeof(buffer), 0);
    char* end;
    int client_ack= strtol(buffer,&end,10);
    std::cout << "client_ack:" << client_ack << std::endl;
    if(client_ack == this->syn + 1){
        this->syn++;
        std::cout << "ack received" << std::endl;
    }
    else send_syn_ack();
}

void serverThread::end_simulation(SOCKET init_socket) {
    closesocket(init_socket);
    WSACleanup();
}

void serverThread::send_ack(){
    std::string message = std::to_string((this->client_syn + 1));
    send(this->server_socket,message.c_str(),strlen(message.c_str()),0);
    this->client_syn ++;
}


void serverThread::recv_message(){
    bool exit = false;
    while(!exit){
        char buffer[1024] = {0};
        recv(this->server_socket, buffer, sizeof(buffer), 0);
        exit = !handle_message(buffer);
        send_ack();
    }
    end_simulation(this->server_socket);
}
void replaceCharacter(char* str, char c1, char c2) {
    int j, n = strlen(str);
    for (int i = j = 0; i < n; i++) {
        if (str[i] != c1)
            str[j++] = str[i];
        else
            str[j++] = c2;
    }
    str[j] = '\0';
}

bool serverThread::handle_message(char* buffer){
    replaceCharacter(buffer,'$',' ');
    char* end;
    int client_number = strtol(buffer, &end, 10);
    int packet_number = strtol(end,&end,10);
    int total_packet = strtol(end,&end,10);
    std::cout << "client number: " << client_number;
    std::cout << " packet number: " << packet_number;
    std::cout << " total packets: " << total_packet << std::endl;
    std::cout << "message: " << end << std::endl;
    if(std::string(end).compare("quit")==0){
        return true;
    }

}
