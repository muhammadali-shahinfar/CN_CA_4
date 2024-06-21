#include "slidingwindowsender.h"

#include <QEventLoop>
#include <QFile>
#include <QTimer>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>
#include <winsock2.h>
#include <QThread>
#include <QDir>

SlidingWindowSender::SlidingWindowSender(int n,int go_back_N,QObject *parent)
    : client{n,false,parent}
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }

    SOCKET router = socket(AF_INET, SOCK_DGRAM, 0);
    if (router == INVALID_SOCKET) {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    this->client_socket = router;

    struct sockaddr_in host_address;
    memset(&host_address, 0, sizeof(host_address));
    host_address.sin_family = AF_INET;
    host_address.sin_port = htons(8084 + n);
    host_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Assuming the host is on local machine
    this->sync_value = 0;
    char buffer[1024];
    itoa(this->sync_value,buffer,10);
    int result = sendto(router, (char*) buffer, 1700, 0, (const struct sockaddr*)&host_address,sizeof(host_address));
    if (result == SOCKET_ERROR) {
        printf("sendto failed with error: %d\n", WSAGetLastError());
    }
    else {
        printf("Bytes sent: %d\n", result);
    }

    int recv_len = recvfrom(router,buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr*)&host_address, &result);
    std::cout << buffer << std::endl;
    this->server_address = &host_address;
    char* end;
    int ack = strtol(buffer,&end,10);
    // QThread::sleep(10);
    QString path = QString("lorem_ipsum.txt");
    send_file(path);
    // struct sockaddr_in client_address;
    // len = sizeof(client_address);
    // while(true){
    //     char abbas[1024];
    //     int n = recvfrom(router, (char*)abbas, 1700, 0, (struct sockaddr *)&client_address,&len );
    //     if (n == SOCKET_ERROR) {
    //         printf("recvfrom failed with error: %d\n", WSAGetLastError());
    //     }
    //     else {
    //         abbas[n] = '\0';  // Null-terminate the received data
    //         std::cout << "Client: " << abbas << std::endl;
    //     }
    // }

    // closesocket(router);
    // WSACleanup();

}
void SlidingWindowSender::send_file(QString path){
    QString fullPath = QDir::current().absoluteFilePath("lorem_ipsum.txt");
    fullPath.replace("build-CN_CA_4-Desktop_Qt_6_6_2_MinGW_64_bit-Debug/","CN_CA_4/");
    QFile file(fullPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cout << fullPath.toStdString() << std::endl;
        std::cout << "Error: " << file.errorString().toStdString() << std::endl;
        return;
    }
    int first_syn = this->sync_value;
    while(!file.atEnd()){
        while(output_hash.size() < this->go_back_N){
            char* buffer = new char[1700];
            file.read(buffer,1536);
            this->output_hash.append(std::string(buffer));
            std::cout << buffer << std::endl;

        }
        send_buffer();
        wait(500);
        handle_window();
    }
}
void SlidingWindowSender::wait(int time)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(time);
    loop.exec();
}

void SlidingWindowSender::handle_window()
{
    for(int i=0;i<this->go_back_N;i++){
        receive_ack();
    }
    if(this->last_ack_received == this->sync_value)
        this->output_hash.clear();
    else{
        int dropped_packet = this->sync_value - this->last_ack_received;
        this->sync_value = this->last_ack_received;
        send_buffer(0);
    }
}

void SlidingWindowSender::send_buffer(int l){
    socklen_t clilen;
    for(int i=l;i<this->output_hash.size();i++){
        sendto(this->client_socket,this->output_hash[i].c_str(),strlen(this->output_hash[i].c_str()),0,(const sockaddr*) this->server_address,clilen);
    }
}

void SlidingWindowSender::receive_ack(){
    sockaddr_in addr = this->create_sockaddr_in(8084 + this->client_number);
    socklen_t len;
    char * buffer = new char[1700];
    int n = recvfrom(this->client_socket,(char*)buffer,1536,MSG_WAITALL, (struct sockaddr *) &this->server_address, &len);
    buffer[n] = ' ';
    char* end;
    int received_ack = strtol(buffer,&end,10);
    if(this->last_ack_received < received_ack){
        this->last_ack_received = received_ack;
    }
    if(received_ack == 0){
        std::cout << buffer << std::endl;
        this->send_ack();
    }
    delete buffer;

}
