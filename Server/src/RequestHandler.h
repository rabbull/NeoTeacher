//
// Created by karl on 18-7-31.
//

#ifndef SERVER_REQUESTHANDLER_H
#define SERVER_REQUESTHANDLER_H


#include "TransmissionControlProtocolSerial.h"
#include "ClientManager.h"
#include "Request/LoginRequest.h"
#include "Request/HandInRequest.h"
#include "Request/HandUpRequest.h"

class RequestHandler {
private:
    int connectFileDescriptor;
    TransmissionControlProtocolSerial *TCPSerial;
    pthread_t pid;
    ClientManager *clientManager;

public:
    RequestHandler(int connfd, TransmissionControlProtocolSerial *TCPSerial, pthread_t pid, ClientManager *manager);

    void handle(Request *request);

    void userLogin(LoginRequest *request);

    void userHandUp(HandUpRequest *request);

    void userHandIn(HandInRequest *request);
};


#endif //SERVER_REQUESTHANDLER_H
