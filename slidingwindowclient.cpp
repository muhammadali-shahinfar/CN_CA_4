#include "slidingwindowclient.h"

#include <QEventLoop>
#include <QFile>
#include <QTimer>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>

SlidingWindowClient::SlidingWindowClient(int n,int go_back_N,QObject *parent)
    : client{n,false,parent}
{
    this->go_back_N = go_back_N;
}
void SlidingWindowClient::send_file(QString path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    int first_syn = this->sync_value;
    while(!file.atEnd()){
        while(output_buffer.size() < this->go_back_N){
            char* buffer = new char[1700];
            file.read(buffer,1536);
            this->output_buffer.append(std::string(buffer));
        }
        send_buffer();
        wait(500);
        handle_window();
    }
}
void SlidingWindowClient::wait(int time)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(time);
    loop.exec();
}

void SlidingWindowClient::handle_window()
{
    if(this->last_ack_received == this->sync_value)
        this->output_buffer.clear();
    else{
        int dropped_packet = this->sync_value - this->last_ack_received;
        this->sync_value = this->last_ack_received;
        send_buffer(this->go_back_N - dropped_packet);
    }
}

void SlidingWindowClient::send_buffer(int l){
    sockaddr_in addr = this->create_sockaddr_in();

    socklen_t clilen;
    for(int i=l;i<this->output_buffer.size();i++){
        sendto(this->client_socket,this->output_buffer[i].c_str(),strlen(this->output_buffer[i].c_str()),0, (struct sockaddr *)&addr,clilen);
    }
}

void SlidingWindowClient::receive_ack(){
    sockaddr_in addr = this->create_sockaddr_in();
    socklen_t len;
    char * buffer = new char[1700];
    int n = recvfrom(this->client_socket,(char*)buffer,1536,MSG_WAITALL, (struct sockaddr *) &addr, &len);
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

}
