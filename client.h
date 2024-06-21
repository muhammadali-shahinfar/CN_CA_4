#ifndef CLIENT_H
#define CLIENT_H
#include <QList>
#include <string>
#include <winsock2.h>
#include <QObject>

class client : public QObject {
    // Q_OBJECT
public:
    client(int n,bool use_tcp=true,QObject *parent=nullptr);
    void start_messaging();


protected:

    SOCKET client_socket;
    int sync_value;
    int server_sync_value;
    int client_number;
    sockaddr_in create_sockaddr_in(int port);
    void send_SYN();
    bool recv_ack();
    void recv_ack_syn();
    void send_ack();

private:
    bool use_TCP;
    void send_message();
    void end_simulation();
    QList<std::string> create_packets(std::string message);
};

#endif // CLIENT_H
