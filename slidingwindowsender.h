#ifndef SlidingWindowSender_H
#define SlidingWindowSender_H

#include "client.h"

#include <QBuffer>

class SlidingWindowSender : public client
{
public:
    explicit SlidingWindowSender(int n,int go_back_N,QObject *parent = nullptr);
    void send_file(QString path);
private:
    QList<std::string> output_hash;
    void extracted();
    void send_buffer(int l = 0);
    int go_back_N;
    int last_ack_received;
    void receive_ack();
    struct sockaddr_in* server_address;


    void wait(int time);
    void handle_window();
};

#endif // SlidingWindowSender_H
