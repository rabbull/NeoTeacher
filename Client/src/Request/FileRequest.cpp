//
// Created by karl on 18-7-31.
//

#include "FileRequest.h"

FileRequest::FileRequest(int8_t data[], size_t size) {
    packData(data, size);
    setRequestType(RTYPE_FILE);
    calculateCheckSum();
    calculateSize();
}

void FileRequest::packData(int8_t data[], size_t size) {
    requestBody.assign((char *) data, size);
}

void FileRequest::depackData(int8_t **dataBuffer, size_t *sizeBuffer) {
    *sizeBuffer = (size_t) getRequestSize();
    for (int i = 0; i < getRequestSize(); i++) {
        (*dataBuffer)[i] = requestBody.at((unsigned long) i);
    }
}
