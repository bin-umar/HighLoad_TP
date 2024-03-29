//
// Created by jahongir on 9/18/19.
//

#ifndef HIGHLOAD_TP_REQUEST_H
#define HIGHLOAD_TP_REQUEST_H

#include "Config.h"
#include "Status.h"

class Request {
public:
    string connection_type;
    string method;
    string uri;
    string http_version;
    string hostname;
    string user_agent;
    string accept;
    string referer;
    string filename;

    Request(string __root);
    void ParseHttpQuery(char *);
    void ParseUri(char *, char *, size_t);

private:
    void set_hostname(string&);
    void set_connection_type(string&);
    void set_user_agent(string&);
    void set_accept(string&);
    void set_referer(string&);

    bool CheckHeader(const string&, const string&);
};

#endif //HIGHLOAD_TP_REQUEST_H
