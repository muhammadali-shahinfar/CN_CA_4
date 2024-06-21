#ifndef ROUTER_H
#define ROUTER_H

#include "server.h"
#include <QUdpSocket>
#include "slidingwindowserverthread.h"
class router : public Server
{
public:
    explicit router(QObject *parent = nullptr);
private:
};

#endif // ROUTER_H
