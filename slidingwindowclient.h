#ifndef SLIDINGWINDOWCLIENT_H
#define SLIDINGWINDOWCLIENT_H

#include "client.h"

#include <QBuffer>

class SlidingWindowClient : public client
{
public:
    explicit SlidingWindowClient(int n,int go_back_N,QObject *parent = nullptr);
    void send_file(QString path);
private:
    QList<std::string> output_buffer;
    void extracted();
    void send_buffer(int l = 0);
    int go_back_N;
    int last_ack_received;
    void receive_ack();


    void wait(int time);
    void handle_window();
};

#endif // SLIDINGWINDOWCLIENT_H
