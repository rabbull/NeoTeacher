//
// Created by karl on 18-7-29.
//

#include <zconf.h>
#include <sys/stat.h>
#include <stdexcept>
#include <cstring>
#include "HandInRequest.h"

HandInRequest::HandInRequest(std::string path) {
    packData(std::move(path));
    setRequestType(RTYPE_HANDIN);
    calculateCheckSum();
    calculateSize();
}

void HandInRequest::packData(std::string path) {
    struct stat statBuffer = {0};
    if (stat(path.c_str(), &statBuffer) < 0) {
        throw std::runtime_error(std::string ("failed reading information of file at \"") + path + std::string("\""));
    }
    requestBody.assign((char*)&statBuffer, sizeof(statBuffer));
}

void HandInRequest::depackData(struct stat *statBuffer) {
    memcpy(statBuffer, requestBody.c_str(), sizeof(struct stat));
}
