#include "client.h"
#include "router.h"
#include "senderclient.h"
#include "server.h"

// congest
// #include "congest_client.cpp"
// #include "congest_server.cpp"

// #include <QCoreApplication>
// #include <iostream>

// int main(int argc, char *argv[]) {
//     QCoreApplication a(argc, argv);

//     int i;
//     std::cout<<"To run server enter 0 and for clilent enter 1"<<std::endl;
//     std::cin >> i;
//     if(i ==0){
//         Server* server = new Server();
//         server->new_connection();
//     }
//     else{
//         client* new_client = new client(i);
//         new_client->start_messaging();

//     }

//     return a.exec();
// }


#include <QCoreApplication>
#include "receiverClient.h"
#include <iostream>
#include <QDir>
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    int i;
    std::cout<<"To run server enter 0 and for sender enter 1 and for receiver enter 2"<<std::endl;
    std::cin >> i;
    if(i ==0){
        router* server = new router();
    }
    if(i==1){
        SenderClient* sender = new SenderClient(i);
        std::cin >> i;
        QString fullPath = QDir::current().absoluteFilePath("lorem_ipsum.txt");
        fullPath.replace("/build-CN_CA_4-Desktop_Qt_6_6_2_MinGW_64_bit-Debug","/CN_CA_4");
        sender->send_file(fullPath);

    } if(i==2){
        receiverClient* receiver = new receiverClient(i);
        receiver->receive_file();
    }

    return a.exec();
}

// using namespace std;
// // congest
// int main(int argc, char *argv[]) {
//     QCoreApplication a(argc, argv);
//     srand(time(nullptr));

//     Server server;
//     Client client;

//     cout << "Scenario 1: No packet loss" << endl;
//     server.simulate(100);

//     cout << "\nScenario 2: Frequent packet loss" << endl;
//     srand(42); // Seed for reproducible packet loss
//     server.simulate(100);

//     cout << "\nScenario 3: Client simulation" << endl;
//     client.simulate(10);

//     return a.exec();
// }



