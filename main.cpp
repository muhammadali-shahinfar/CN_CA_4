#include "client.h"
#include "router.h"
#include "server.h"
#include "slidingwindowsender.h"

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
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    int i;
    std::cout<<"To run server enter 0 and for clilent enter 1"<<std::endl;
    std::cin >> i;
    if(i ==0){
        router* server = new router();
        // server->new_connection();
    }
    else{
        SlidingWindowSender* new_client = new SlidingWindowSender(i,8);
        // new_client->start_messaging();

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



