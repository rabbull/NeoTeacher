//
// Created by karl on 18-7-31.
//

#ifndef CLIENT_USEROPERATIONS_H
#define CLIENT_USEROPERATIONS_H


#include "TransmissionControlProtocolSerial.h"

void checkUsername(std::string&);

namespace UserOperations {
    void Login(TransmissionControlProtocolSerial* TCPSerial);

    void HandIn(TransmissionControlProtocolSerial* TCPSerial);

    void HandUp(TransmissionControlProtocolSerial* TCPSerial);

    void Quit(TransmissionControlProtocolSerial* TCPSerial);
};


#endif //CLIENT_USEROPERATIONS_H
