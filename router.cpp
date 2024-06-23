#include "router.h"
#include <iostream>
#include <winsock2.h>
#include "defs.h"
void router::bind_socket()
{
    if (bind(server_socket, (const struct sockaddr*)&this->server_address, sizeof(this->server_address)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void router::create_socket()
{
    this->server_socket
        = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->server_socket == INVALID_SOCKET) {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

router::router(QObject *parent)
{

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }
    create_socket();
    this->create_sockaddr_in(65258);;
    bind_socket();

    this->handle_router();



    closesocket(server_socket);
    WSACleanup();
}
void router::create_sockaddr_in(int port){
    memset(&this->server_address, 0, sizeof(this->server_address));
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = port;
}

void router::handle_router(){
    std::string message;
    struct sockaddr_in client_address;
    while(true){
        std::cout << "1";
        message = this->receive_message(client_address);
        char* remain_buffer;
        int type = strtol(message.c_str(), &remain_buffer,10);
        if(type==0){
            this->first_client_connection(remain_buffer, client_address);
        }
        else if(type==1){
            this->handle_message(remain_buffer);
        }
    }
}
void router::first_client_connection(std::string message, struct sockaddr_in client_address){
    int host_num = strtol(message.c_str(), nullptr, 10);
    if(host_num == 1){
        this->host_A_address =client_address;
        std::cout << "host A connected" << std::endl;
        this->send_message("connection accepted", 1);
    } else{
        this->host_B_address = client_address;
        std::cout << "host B connected" << std::endl;
        this->send_message("connection accepted", 2);
    }
}
void router::send_message(std::string message, int destination_host){
    if(destination_host == 2){ // reverse, we fill destination with host id, so it is src_id :)
        sendto(this->server_socket, message.c_str(), message.length(), 0, (const struct sockaddr *)&this->host_A_address, sizeof(this->host_A_address));
    }
   else if(destination_host == 1){
        sendto(this->server_socket, message.c_str(), message.length(), 0, (const struct sockaddr *)&this->host_B_address, sizeof(this->host_B_address));
    }
}
std::string router::receive_message(struct sockaddr_in& client_address){
    char buffer[1700];
    socklen_t len = sizeof(client_address);
    int n = recvfrom(this->server_socket, (char*)buffer, 1700, 0, (struct sockaddr*)&client_address, &len);
    buffer[n] = '\0';
    std::cout << buffer << "wew" << std::endl;
    return std::string(buffer);
}

void router::handle_message(std::string message){
    char* buffer;
    int destination = strtol(message.c_str(),&buffer, 10);
    this->send_message(buffer, destination);
}
