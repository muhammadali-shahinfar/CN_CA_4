#ifndef RECEIVERCLIENT_H
#define RECEIVERCLIENT_H

#include "UDPClient.h"

#include <QMap>

class receiverClient : public UDPClient
{
public:
    explicit receiverClient(int host_id, QObject *parent = nullptr);
    void receive_file();
private:
    int last_part_received;
    bool handle_message(std::string message);
    QMap<int, std::string> received_message;
};

#endif // RECEIVERCLIENT_H
