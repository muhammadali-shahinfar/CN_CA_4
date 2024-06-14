#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <winsock2.h>

class serverThread : public QThread
{
public:
    explicit serverThread(QObject *parent = nullptr);
    void run() override;
private:
    void recv_syn();
    void send_syn_ack();
    void recv_ack();
    int syn;
    int client_syn;
    SOCKET server_socket;
};

#endif // SERVERTHREAD_H
