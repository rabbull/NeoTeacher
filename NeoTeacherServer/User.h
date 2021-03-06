//
// Created by karl on 18-7-27.
//

#ifndef NEOTEACHERCLIENT_USER_H
#define NEOTEACHERCLIENT_USER_H


#include <cstdint>
#include <netinet/in.h>
#include "Client.h"

class User {
private:
    uint32_t userId;
    Client client;

public:
    User(uint32_t id, sockaddr_in address, int connfd);
};

#endif //NEOTEACHERCLIENT_USER_H
