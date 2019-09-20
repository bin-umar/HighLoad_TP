//
// Created by jahongir on 9/17/19.
//

#ifndef HIGHLOAD_TP_RESPONSE_H
#define HIGHLOAD_TP_RESPONSE_H

#include "Status.h"
#include "Config.h"
#include "uv.h"

class Response {
public:
    Response();
    void Status(int __st);
    void Send(int __fd, const string& __data);

private:
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
    void SendFile(int __fd, const string& __filename);
};

#endif //HIGHLOAD_TP_RESPONSE_H
