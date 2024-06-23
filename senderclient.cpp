#include "senderclient.h"
#include <iostream>
#include <QFile>

SenderClient::SenderClient(int host_id,QObject *parent)
    : UDPClient{host_id, parent}
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }
    this->create_socket();
    this->create_sockaddr_in(65258);
    this->send_message("salam", 0);

}
void SenderClient::send_file(QString path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cout << path.toStdString() << std::endl;
        return;
    }
    int i=1;
    int last_recived_message = 0;
    while(!file.atEnd()){
        while(output_buffer.size() < this->go_back_n){
            char* buffer = new char[1700];
            file.read(buffer,1536);
            std::string message = std::to_string(i) + " " + std::string(buffer);
            this->output_buffer.append(message);
        }
        for(int j=0;j<output_buffer.size();j++){
            this->send_message(output_buffer[i],1);
        }
        std::string received_acks = this->receive_message();
        std::cout << "acks: " << received_acks << std::endl;
        last_recived_message = this->find_last_acks(received_acks);
        int buffer_size = this->output_buffer.size();
        while(this->output_buffer.size() == buffer_size - last_recived_message){
            this->output_buffer.removeFirst();
        }
        Sleep(10);

    }
    this->send_message("&&$end$&&",1);
    WSACleanup();

}
void SenderClient::set_go_back_n(int n){
    this->go_back_n = n;
}
int SenderClient::find_last_acks(std::string received_acks){
    char* end;
    int last_ack = strtol(received_acks.c_str(),&end,10);
    int last_ack_tmp = 2;
    while(last_ack_tmp){
        last_ack_tmp = strtol(end,&end,10);
        if(last_ack_tmp > last_ack){
            last_ack = last_ack_tmp;
        }
    }
    return last_ack;
}
