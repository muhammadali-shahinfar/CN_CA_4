#include "receiverclient.h"

receiverClient::receiverClient(int host_id,QObject *parent)
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
    this->last_part_received = 0;
}
void receiverClient::receive_file(){
    while(true){
        std::string message = this->receive_message();
        if(message != "TIMEOUT"){
            if(this->handle_message(message))
                break;
        }
    }
    WSACleanup();

}

bool receiverClient::handle_message(std::string message){
    char* remain_message;
    int type = strtol(message.c_str(), &remain_message, 10);
    int host_id = strtol(remain_message, &remain_message, 10);
    if(std::string(remain_message).compare(" &&$end$&&")) // should be checked
        return true;
    int message_syn = strtol(remain_message,&remain_message,10);
    this->received_message.insert(message_syn,std::string(remain_message));
    this->send_message(" " + std::to_string(message_syn), 1);
}


