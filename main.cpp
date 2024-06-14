#include "client.h"
#include "server.h"

#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int i;
    std::cin >> i;
    if(i ==0){
        Server* server = new Server(3);
    }
    else{
        client* new_client = new client(i);
    }

    return a.exec();
}
