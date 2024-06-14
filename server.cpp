#include "server.h"
#include "serverthread.h"

#include <Qthread>
#include <winsock.h>
Server::Server(int n,QObject *parent)
    : QObject{parent}
{


    for(int i=0;i<n;i++){
        serverThread* thread = new serverThread(this);
        thread->run();
    }
}
