#include "udpclient.h"
#include <sys/time.h>
#include <iostream>
UDPClient::UDPClient(int host_id,QObject *parent)
    : QObject{parent}
{
    this->host_id = host_id;
}
void UDPClient::create_socket()
{
    this->client_socket= socket(AF_INET, SOCK_DGRAM, 0);
    if (this->client_socket == INVALID_SOCKET) {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}
void UDPClient::create_sockaddr_in(int port){
    memset(&this->address, 0, sizeof(this->address));
    this->address.sin_family = AF_INET;
    address.sin_port = htons(port);
    this->address.sin_addr.s_addr = inet_addr("127.0.0.1");
}
void UDPClient::send_message(std::string message, int type){
    message  = std::to_string(type) + " " + std::to_string(this->host_id) + " " + message;
    sendto(this->client_socket, message.c_str(), message.length(), 0, (struct sockaddr*)&address, sizeof(this->address));
    std::cout << message;
}
std::string UDPClient::receive_message(){
    char buffer[1700];
    socklen_t len;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(this->client_socket, &readfds);
    timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    int retval = select(this->client_socket + 1, &readfds, NULL, NULL, &timeout);
    if(retval == -1){
        printf("Error creating retval: %ld\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    } else if(retval){
        int n = recvfrom(this->client_socket, (char*) buffer, 1700, 0, (struct sockaddr*)&this->server_Address,&len);
        buffer[n] = '\0';
        return std::string(buffer);
    }
    else
        return "TIMEOUT";

}
