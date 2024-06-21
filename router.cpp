#include "router.h"

router::router(QObject *parent)
    : Server{parent}
{

    // this->host_A.bind(QHostAddress::LocalHost, 8085);
    // this->host_B.bind(QHostAddress::LocalHost, 8086);
    // connect(&host_A, SIGNAL(readyRead()), this, SLOT(read_ready_A()));
    // connect(&host_B, SIGNAL(readyRead()), this, SLOT(read_ready_B()));
}
void router::read_ready_A(){
    // QByteArray buffer;
    // buffer.resize(host_A.pendingDatagramSize());
    // QHostAddress sender;
    // quint16 port;
    // host_A.readDatagram(buffer.data(), buffer.size(), &sender, &port);
    // QByteArray client_data;
    // client_data.append(buffer);
    // host_B.writeDatagram(client_data,QHostAddress::LocalHost, 8086);
}

