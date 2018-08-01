//
// Created by karl on 18-7-28.
//

#ifndef NEOTEACHERSERVER_CLIENTMANAGER_H
#define NEOTEACHERSERVER_CLIENTMANAGER_H


#include <vector>
#include "Client.h"
#include "TransmissionControlProtocolSerial.h"

#include "Request/OKRequest.h"


class ClientManager {
private:
    std::vector<Client *> clientList;
    pthread_t newConnectionHandlerPid;

public:
    ClientManager() = default;

    void newClient(int connectFileDescriptor, sockaddr_in clientAddr, pthread_t pid);

    Client *findClientByConnFD(int connectFileDescriptor);

    void handleNewConnection(TransmissionControlProtocolSerial *TCPSerial);
};


#endif //NEOTEACHERSERVER_CLIENTMANAGER_H
