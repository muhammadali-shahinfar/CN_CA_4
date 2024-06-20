#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <winsock2.h>
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

private:
    void connect_to_client();
    sockaddr_in create_sockaddr_in();


signals:
};

#endif // SERVER_H
