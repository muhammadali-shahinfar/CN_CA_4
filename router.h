#ifndef ROUTER_H
#define ROUTER_H
#include<winsock2.h>
#include<iostream>
#include <qlist.h>
class router
{
public:
    explicit router(QObject *parent = nullptr);
private:
    void create_sockaddr_in(int port);
    QList<std::string> host_A_output_buffer;
    QList<std::string> host_B_output_buffer;
    struct sockaddr_in host_A_address;
    struct sockaddr_in host_B_address;
    struct sockaddr_in server_address;
    SOCKET server_socket;

    void bind_socket();
    void handle_router();
    std::string receive_message(struct sockaddr_in& client_address);
    void handle_message(std::string message);
    void first_client_connection(std::string message, struct sockaddr_in client_address);
    void send_message(std::string message, int destination_host);
    void create_socket();
};

#endif // ROUTER_H
