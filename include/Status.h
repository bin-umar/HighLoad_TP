//
// Created by jahongir on 9/17/19.
//

#ifndef HIGHLOAD_TP_STATUS_H
#define HIGHLOAD_TP_STATUS_H

#include <map>

#define HTTP_STATUS_OK                  200
#define HTTP_STATUS_FORBIDDEN           403
#define HTTP_STATUS_NOT_FOUND           404
#define HTTP_STATUS_METHOD_NOT_ALLOWED  405

const std::map<const unsigned short, std::string> status_phrase = {
    { HTTP_STATUS_OK, "OK" },
    { HTTP_STATUS_FORBIDDEN, "FORBIDDEN" },
    { HTTP_STATUS_NOT_FOUND, "NOT FOUND" },
    { HTTP_STATUS_METHOD_NOT_ALLOWED, "METHOD NOT ALLOWED" },
};

#endif //HIGHLOAD_TP_STATUS_H
