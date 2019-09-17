//
// Created by jahongir on 9/17/19.
//

#ifndef HIGHLOAD_TP_RESPONSE_H
#define HIGHLOAD_TP_RESPONSE_H

#include <iostream>
#include <ctime>
#include <cstring>
#include <sys/socket.h>

#include "../include/Status.h"
#include "uv.h"

using std::cout;
using std::endl;
using std::string;

class Response {
public:
    Response();
    void Status(int __st);
    void Send(int __fd, const string& __data);

private:
    string content;
    string content_type;
    string connection_type;
    size_t content_length;
    string date;
    string data;
    string http_version;
    string phrase;
    string server;
    string status;

    static string getCurrentDateGMT();
    size_t get_size();
    void set_data();
};

#endif //HIGHLOAD_TP_RESPONSE_H
