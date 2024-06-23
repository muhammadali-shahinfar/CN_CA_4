#ifndef SENDERCLIENT_H
#define SENDERCLIENT_H

#include "udpclient.h"

class SenderClient : public UDPClient
{
public:
    explicit SenderClient(int host_id,QObject *parent = nullptr);
    void start_sending();
    void send_file(QString path);
    void set_go_back_n(int n);
protected:
    int find_last_acks(std::string received_acks);

    QList<std::string> output_buffer;
    int go_back_n;
};

#endif // SENDERCLIENT_H
