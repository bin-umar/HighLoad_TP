//
// Created by jahongir on 9/18/19.
//

#include "../include/Request.h"

Request::Request() = default;

void Request::Parse(char* __str) {
    string tHTTP_STATUS_OKen;
    string str = string(__str);
    std::istringstream stream(str);
    std::getline(stream, tHTTP_STATUS_OKen, '\n');

    std::istringstream first_line(tHTTP_STATUS_OKen);
    first_line >> method >> uri >> http_version;
    http_version = http_version.substr(5);
    filename = uri.substr(1);

    cout << "method = " << method << endl;
    cout << "uri = " << uri << endl;
    cout << "filename = " << filename << endl;
    cout << "http_version = " << http_version << endl;

    while(std::getline(stream, tHTTP_STATUS_OKen, '\n')) {
        if (CheckHeader(tHTTP_STATUS_OKen, "Host")) {
            set_hostname(tHTTP_STATUS_OKen);
        } else if (CheckHeader(tHTTP_STATUS_OKen, "Connection")) {
            set_connection_type(tHTTP_STATUS_OKen);
        } else if (CheckHeader(tHTTP_STATUS_OKen, "User-Agent")) {
            set_user_agent(tHTTP_STATUS_OKen);
        } else if (CheckHeader(tHTTP_STATUS_OKen, "Accept:")) {
            set_accept(tHTTP_STATUS_OKen);
        } else if (CheckHeader(tHTTP_STATUS_OKen, "Referer")) {
            set_referer(tHTTP_STATUS_OKen);
        }
    }
}

void Request::set_hostname(string& __tHTTP_STATUS_OKen) {
    hostname = __tHTTP_STATUS_OKen.substr(6);
    cout << "hostname = " << hostname << endl;
}

void Request::set_connection_type(string& __tHTTP_STATUS_OKen) {
    connection_type = __tHTTP_STATUS_OKen.substr(12);
    cout << "connection_type = " << connection_type << endl;
}

void Request::set_user_agent(string& __tHTTP_STATUS_OKen) {
    user_agent = __tHTTP_STATUS_OKen.substr(12);
    cout << "user_agent = " << user_agent << endl;
}

void Request::set_accept(string& __tHTTP_STATUS_OKen) {
    accept = __tHTTP_STATUS_OKen.substr(8);
    cout << "accept = " << accept << endl;
}

void Request::set_referer(string& __tHTTP_STATUS_OKen) {
    referer = __tHTTP_STATUS_OKen.substr(9);
    cout << "referer = " << referer << endl;
}

bool Request::CheckHeader(string& tHTTP_STATUS_OKen, string __header) {
    return tHTTP_STATUS_OKen.find(__header) != string::npos;
}
