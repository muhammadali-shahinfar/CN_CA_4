#include "client.h"
#include <QRandomGenerator>
#include <iostream>
#include <algorithm>

client::client(int n) {

    this->client_number = n;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }

    this->client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8025 + n);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // sending connection request
    connect(client_socket, (struct sockaddr *)&serverAddress,
            sizeof(serverAddress));

    send_SYN();
    recv_ack_syn();
    send_ack();
    send_message();

}

void client::send_SYN(){
    int random_value = QRandomGenerator::global()->generate();
    std::string message = std::to_string((random_value));
    send(this->client_socket,message.c_str(),strlen(message.c_str()),0);
    this->sync_value = random_value;
    std::cout << "client SyN:" << random_value << std::endl;
}
void client::recv_ack_syn(){
    char buffer[1024] = {0}; // maybe has bug
    recv(this->client_socket,buffer, sizeof(buffer), 0);
    std::cout << "server ack buffer:" << buffer << std::endl;
    char* end;
    int ack, server_syn;
    server_syn = strtol(buffer,&end,10);
    ack = strtol(end,&end,10);

    if(this->sync_value +1 == ack){
        this->sync_value++;
        this->server_sync_value = server_syn;
    }
    else
        send_SYN();
}
bool client::recv_ack(){
    char buffer[1024] = {0};
    char* end;
    recv(this->client_socket, buffer, sizeof(buffer), 0);
    int ack = strtol(buffer, &end,10);
    std::cout << this->sync_value << " " << ack << std::endl;
    if(this->sync_value + 1 == ack){
        std::cout << "ack received" << std::endl;
        this->sync_value++;
        return true;
    }
    else
        std::cout << "ack didnt received" << std::endl;
        return false;
}
void client::send_ack(){
    int ack = this->server_sync_value + 1;
    std::string message = std::to_string((ack));
    send(this->client_socket, message.c_str(),strlen(message.c_str()), 0);
    std::cout << message << std::endl;

}

void client::send_message(){
    while(true){
        std::string message;
        getline(std::cin,message);
        QList<std::string> packets = create_packets(message);
        for(int i=0;i<packets.size();i++){

            send(this->client_socket,packets[i].c_str(),strlen(packets[i].c_str()),0);
            while(!recv_ack()){
                send(this->client_socket,packets[i].c_str(),strlen(packets[i].c_str()),0);
            }

        }
        if(message.compare("quit")==0){
            end_simulation();
            break;
        }
    }


}

QList<std::string> client::create_packets(std::string message){
    std::replace(message.begin(), message.end(), ' ', '$');
    QList<std::string> string_packets;
    for(int i=0;i < (message.size()/10);i++){
        string_packets.append( std::to_string(this->client_number) +" "+ std::to_string(i) + " " +  std::to_string(((message.size()/10) + 1)) + " " + message.substr(i*10,10));
    }
    if(message.size() % 10 != 0){
        string_packets.append( std::to_string(this->client_number) + " " + std::to_string((message.size()/10)) + " " + std::to_string((message.size()/10) + 1) + " " + message.substr((message.size() - (message.size()%10)), message.size()%10));
    }
    return string_packets;
}
void client::end_simulation() {
    closesocket(client_socket);
    WSACleanup();
}

