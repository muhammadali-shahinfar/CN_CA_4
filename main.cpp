#include "client.h"
#include "server.h"

#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    int i;
    std::cout<<"To run server enter 0 and for clilent enter 1"<<std::endl;
    std::cin >> i;
    if(i ==0){
        Server* server = new Server(3);
    }
    else{
        client* new_client = new client(i);
        client* new_client1 = new client(i);
        client* new_client2 = new client(i);

    }

    return a.exec();
}
