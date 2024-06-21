#ifndef ROUTER_H
#define ROUTER_H

#include "server.h"
#include <QUdpSocket>

class router : public Server
{
public:
    explicit router(QObject *parent = nullptr);
    // void write_data(const QString&);
    void read_ready_A();
    // void read_ready_B();
private:
    SOCKET host_A;
    SOCKET host_B;
    QList<std::string> output_buffer;
};

#endif // ROUTER_H
