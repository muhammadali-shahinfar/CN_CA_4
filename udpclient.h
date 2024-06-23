#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <winsock2.h>
#include "defs.h"
class UDPClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPClient(int host_id,QObject *parent = nullptr);
protected:
    void create_socket();
    void create_sockaddr_in(int port);
    void send_message(std::string message, int type);
    std::string receive_message();

    SOCKET client_socket;
    struct sockaddr_in server_Address;
    struct sockaddr_in address;
    int host_id;
signals:
};

#endif // UDPCLIENT_H
