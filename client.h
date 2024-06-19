#ifndef CLIENT_H
#define CLIENT_H
#include <QList>
#include <string>
#include <winsock2.h>
#include <QObject>

class client : public QObject {
    // Q_OBJECT
public:
    client(int n,QObject *parent=nullptr);

private:
    SOCKET client_socket;
    int sync_value;
    int server_sync_value;
    int client_number;
    void send_SYN();
    bool recv_ack();
    void recv_ack_syn();
    void send_ack();
    void send_message();
    void end_simulation();
    QList<std::string> create_packets(std::string message);
    sockaddr_in create_sockaddr_in();
    void start_messaging();
};

#endif // CLIENT_H
