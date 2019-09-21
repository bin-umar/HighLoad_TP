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
    explicit Response(int __fd);
    void Status(int __st);
    void SendHeaders();
    void SendFile(const string& __filename);

    string content_type;
    size_t content_length;

private:
    string content;
    string connection_type;
    string date;
    string data;
    string http_version;
    string phrase;
    string server;
    string status;
    int fd;

    static string getCurrentDateGMT();
    size_t get_size();
    void set_data();
};

#endif //HIGHLOAD_TP_RESPONSE_H
