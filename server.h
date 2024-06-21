#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <winsock2.h>
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void new_connection();

    SOCKET connection_detail(int port);

protected:
    void connect_to_client();
    sockaddr_in create_sockaddr_in(int port);


signals:
};

#endif // SERVER_H
