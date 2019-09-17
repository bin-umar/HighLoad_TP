//
// Created by jahongir on 9/17/19.
//

#ifndef HIGHLOAD_TP_STATUS_H
#define HIGHLOAD_TP_STATUS_H

#include <map>

#define OK                  200
#define FORBIDDEN           403
#define NOT_FOUND           404
#define METHOD_NOT_ALLOWED  405

const std::map<const unsigned short, std::string> status_phrase = {
    { OK, "OK" },
    { FORBIDDEN, "FORBIDDEN" },
    { NOT_FOUND, "NOT FOUND" },
    { METHOD_NOT_ALLOWED, "METHOD NOT ALLOWED" },
};

#endif //HIGHLOAD_TP_STATUS_H
