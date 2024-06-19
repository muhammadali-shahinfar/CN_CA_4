#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <winsock2.h>

class serverThread : public QThread
{
public:
    explicit serverThread(int n,QObject *parent = nullptr);
    void run() override;
private:
    void recv_syn();
    void send_syn_ack();
    void send_ack();
    bool handle_message(char[]);
    void recv_ack();
    void recv_message();
    int syn;
    int client_syn;
    int client_num;
    SOCKET server_socket;
    void end_simulation(SOCKET init_socket);
    sockaddr_in create_sockaddr_in();
    void start_messaging();
};

#endif // SERVERTHREAD_H
