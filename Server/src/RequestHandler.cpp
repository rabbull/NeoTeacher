//
// Created by karl on 18-7-31.
//

#include <stdexcept>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <strings.h>
#include "RequestHandler.h"
#include "Request/ErrorRequest.h"
#include "Request/FileRequest.h"

RequestHandler::RequestHandler(int connfd, TransmissionControlProtocolSerial *TCPSerial, pthread_t pid,
                               ClientManager *manager) {
    connectFileDescriptor = connfd;
    this->TCPSerial = TCPSerial;
    this->pid = pid;
    clientManager = manager;
}

void RequestHandler::handle(Request *request) {
    switch (request->getRequestType()) {
        case Request::RTYPE_QUIT:
            throw std::runtime_error(std::string("quit request"));
        case Request::RTYPE_HANDIN:
            userHandIn((HandInRequest *) request);
            break;
        case Request::RTYPE_HANDUP:
            userHandUp((HandUpRequest *) request);
            break;
        case Request::RTYPE_LOGIN:
            userLogin((LoginRequest *) request);
            break;

        default:
            throw std::runtime_error(std::string("unknown request"));
    }
}

void RequestHandler::userHandUp(HandUpRequest *request) {
}

void RequestHandler::userHandIn(HandInRequest *request) {
    // recv file information
    struct stat fileInfo = {0};
    request->depackData(&fileInfo);

    auto acc = new OKRequest();
    TCPSerial->sendRequest(connectFileDescriptor, acc);

    // recv File
    size_t packSize = 65535;
    auto packNum = (int) (fileInfo.st_size / packSize);
    // specify file location
    std::string localPath;
//    std::cin >> localPath;
    localPath = "/home/karl/Document/temp/file";
    int fd = open(localPath.c_str(), O_WRONLY);
    if (fd == -1) {
        throw std::runtime_error(std::string("cannot write to ") + localPath);
    }
    for (int cnt = 0; cnt < packNum; cnt++) {
        Request * recvRequest;
        TCPSerial->receiveRequest(connectFileDescriptor, &recvRequest);
        auto fileRequest = (FileRequest *) recvRequest;

        char buff[packSize];
        bzero(buff, packSize);
        fileRequest->depackData(buff);

    }
    delete acc;
}

void RequestHandler::userLogin(LoginRequest *request) {
    std::string username;
    std::vector<uint8_t> summary;
    request->depackData(&username, &summary);
    auto errRequest = new ErrorRequest("password not match");
    auto accRequest = new OKRequest();
    if (summary.size() != SHA512_DIGEST_LENGTH) {
        TCPSerial->sendRequest(connectFileDescriptor, errRequest);
    } else {
        TCPSerial->sendRequest(connectFileDescriptor, accRequest);
    }
    delete accRequest;
    delete errRequest;
}
