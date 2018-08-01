//
// Created by karl on 18-7-28.
//

#include <stdexcept>
#include <strings.h>
#include <zconf.h>
#include "ClientManager.h"
#include "Request/LoginRequest.h"
#include "RequestHandler.h"

//void requestHandler(Request *request, bool *quitFlag, TransmissionControlProtocolSerial* TCPSerial, int connfd);

void ClientManager::newClient(int connectFileDescriptor, sockaddr_in clientAddr, pthread_t pid) {
    clientList.push_back(new Client(connectFileDescriptor, clientAddr, pid));
}

typedef struct {
    TransmissionControlProtocolSerial *TCPSerial;
    int connfd;
    pthread_t pid;
    ClientManager *manager;
} connectionListenLoopArg;

void *connectionListenLoop(void *args) {
    auto connfd = ((connectionListenLoopArg *) args)->connfd;
    auto TCPSerial = ((connectionListenLoopArg *) args)->TCPSerial;
    auto pid = ((connectionListenLoopArg *) args)->pid;
    auto clientManager = ((connectionListenLoopArg *) args)->manager;

    auto requestHandler = new RequestHandler(connfd, TCPSerial, pid, clientManager);
    fprintf(stderr, "[STAT] start listening msg in thread [%u]..\n", pid);
    while (true) {
        auto request = new Request();
        try {
            TCPSerial->receiveRequest(connfd, &request);
        } catch (std::runtime_error &error) {
            return nullptr;
        }
        if (request->getRequestType() == Request::RTYPE_QUIT) {
            break;
        } else {
            requestHandler->handle(request);
        }
    }
    auto acc = new OKRequest();
    TCPSerial->sendRequest(connfd, acc);
    delete acc;
    close(connfd);
    fprintf(stderr, "[STAT] stop listening msg in thread [%u]..\n", pid);
    return nullptr;
}

typedef struct {
    TransmissionControlProtocolSerial *TCPSerial;
    ClientManager *clientManager;
} handleNewConnectionLoopArg;

void *handleNewConnectionLoop(void *args) {
    auto listenfd = ((handleNewConnectionLoopArg *) args)->TCPSerial->getListenFileDescriptor();
    auto manager = ((handleNewConnectionLoopArg *) args)->clientManager;
    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int connfd = accept(listenfd, (sockaddr *) &clientAddr, &clientSize);
        if (connfd < 0) {
            throw std::runtime_error(std::string("failed accepting connection"));
        }
        fprintf(stderr, "[STAT] connected with client '%d'..\n", clientAddr.sin_addr.s_addr);
        connectionListenLoopArg arg;
        arg.TCPSerial = ((handleNewConnectionLoopArg *) args)->TCPSerial;
        arg.connfd = connfd;
        arg.manager = manager;
        pthread_create(&arg.pid, nullptr, &connectionListenLoop, &arg);
        manager->newClient(connfd, clientAddr, arg.pid);
    }
}

void ClientManager::handleNewConnection(TransmissionControlProtocolSerial *TCPSerial) {
    handleNewConnectionLoopArg arg;
    arg.clientManager = this;
    arg.TCPSerial = TCPSerial;
    pthread_create(&newConnectionHandlerPid, nullptr, &handleNewConnectionLoop, &arg);
    pthread_join(newConnectionHandlerPid, nullptr);
}
//
//void requestHandler(Request *request, bool *quitFlag, TransmissionControlProtocolSerial* TCPSerial, int connfd) {
//    switch (request->getRequestType()) {
//        case Request::RTYPE_LOGIN:
//            printf ("some one is attempting to log in..\n");
//            TCPSerial->sendRequest(connfd, new OKRequest());
//            break;
//        case Request::RTYPE_HANDUP:
//            break;
//        case Request::RTYPE_QUIT:
//            *quitFlag = true;
//            break;
//        case Request::RTYPE_HANDIN:
//
//        default:
//            throw std::runtime_error(std::string("invalid request type"));
//    }
//}
