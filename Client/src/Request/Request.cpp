//
// Created by karl on 18-7-27.
//

#include <stdexcept>
#include "Request.h"

char toChar(uint8_t u) {
    return (char) ((short) u - 128);
}

uint8_t toUnsignedChar(char c) {
    return (uint8_t) c + (uint8_t) 128;
}

bool Request::check(uint16_t anotherCheckSum) {
    return (checkSum == anotherCheckSum);
}

void Request::serialize(char **serializedRequest) {
    *serializedRequest = new char[requestSize
                                  + 3 /* request size */
                                  + 1 /* request type */
                                  + 2 /* checksum */];

    // serialize request size
    for (int i = 0; i < 4; i++) {
        (*serializedRequest)[i] = toChar((uint8_t) ((requestSize << (8 * i)) >> 24));
    }

    // serialize request type;
    (*serializedRequest)[4] = toChar(requestType);

    // serialize request checksum
    for (int i = 0; i < 2; i++) {
        (*serializedRequest)[5 + i] = toChar((uint8_t) ((checkSum << (8 * i)) >> 8));
    }

    // serialize request body
    for (unsigned long i = 0; i < requestSize; i++) {
        (*serializedRequest)[7 + i] = requestBody.at(i);
    }
}

void Request::disserialize(char *serializedRequest) {
    // load size
    requestSize = 0;
    for (int i = 0; i < 4; i++) {
        requestSize <<= 8;
        requestSize += toUnsignedChar(serializedRequest[i]);
    }

    // load type
    requestType = toUnsignedChar(serializedRequest[4]);

    // load body
    requestBody.assign(serializedRequest + 7, requestSize);

    // load checksum
    checkSum = ((uint16_t) toUnsignedChar(serializedRequest[5])) << 8;
    checkSum += toUnsignedChar(serializedRequest[6]);
    auto otherCheckSum = checkSum;
    calculateCheckSum();
    if (!check(otherCheckSum)) {
        throw std::runtime_error(std::string("failed check"));
    }
}

Request::Request() = default;

Request::Request(uint8_t type, std::string body) {
    setRequestType(type);
    setRequestBody(std::move(body));
    calculateCheckSum();
    calculateSize();
}

void Request::calculateCheckSum() {
    checkSum = 0;
    for (auto c : requestBody) {
        checkSum += c;
    }
}

void Request::calculateSize() {
    requestSize = (uint32_t) requestBody.size();
}

void Request::setRequestBody(std::string body) {
    requestBody = std::move(body);
}

void Request::setRequestType(uint8_t type) {
    requestType = type;
}
