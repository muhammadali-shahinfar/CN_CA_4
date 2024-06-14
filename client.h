#ifndef CLIENT_H
#define CLIENT_H
#include <winsock2.h>


class client
{
public:
    client();
    void send_SYN();
    void recv_ack();
    void send_ack();
private:
    SOCKET client_socket;
    int sync_value;
    int server_sync_value;
};

#endif // CLIENT_H
