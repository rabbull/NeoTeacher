//
// Created by karl on 18-7-31.
//

#ifndef CLIENT_FILEREQUEST_H
#define CLIENT_FILEREQUEST_H


#include <string>
#include "Request.h"

class FileRequest : public Request {
public:
    explicit FileRequest(int8_t data[], size_t size);

    void packData(int8_t data[], size_t size);

    void depackData(int8_t **data, size_t *size);
};

#endif //CLIENT_FILEREQUEST_H
