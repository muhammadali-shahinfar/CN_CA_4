// #include "client.h"
// #include "server.h"
// #include <QCoreApplication>

// int main(int argc, char *argv[]) {
//     QCoreApplication a(argc, argv);

//     int i;
//     std::cout<<"To run server enter 0 and for clilent enter 1"<<std::endl;
//     std::cin >> i;
//     if(i ==0){
//         Server* server = new Server(3);
//     }
//     else{
//         client* new_client = new client(i);
//         client* new_client1 = new client(i);
//         client* new_client2 = new client(i);

//     }

//     return a.exec();
// }


// congest
#include "congest_client.cpp"
#include "congest_server.cpp"

#include <QThread>
#include <QCoreApplication>

class ServerThread : public QThread {
    // Q_OBJECT
public:
    void run() override {
        Server server;
        server.simulate(100);
    }
};

class ClientThread : public QThread {
    // Q_OBJECT
public:
    void run() override {
        Client client;
        client.simulate(100);
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ServerThread serverThread;
    ClientThread clientThread;

    serverThread.start();
    clientThread.start();

    serverThread.wait();
    clientThread.wait();

    return 0;
}

