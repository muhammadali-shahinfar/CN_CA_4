#ifndef SERVER_H
#define SERVER_H

#include <QObject>
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(int n,QObject *parent = nullptr);

private:
    void connect_to_client();

signals:
};

#endif // SERVER_H
