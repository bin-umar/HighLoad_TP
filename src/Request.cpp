//
// Created by jahongir on 9/18/19.
//

#include "../include/Request.h"

Request::Request() = default;

void Request::Parse(char* __str) {
    string token;
    string str = string(__str);
    std::istringstream stream(str);
    std::getline(stream, token, '\n');

    std::istringstream first_line(token);
    first_line >> method >> uri >> http_version;
    http_version = http_version.substr(5);

    cout << "method = " << method << endl;
    cout << "uri = " << uri << endl;
    cout << "http_version = " << http_version << endl;

    while(std::getline(stream, token, '\n')) {
        if (CheckHeader(token, "Host")) {
            set_hostname(token);
        } else if (CheckHeader(token, "Connection")) {
            set_connection_type(token);
        } else if (CheckHeader(token, "User-Agent")) {
            set_user_agent(token);
        } else if (CheckHeader(token, "Accept:")) {
            set_accept(token);
        } else if (CheckHeader(token, "Referer")) {
            set_referer(token);
        }
    }
}

void Request::set_hostname(string& __token) {
    hostname = __token.substr(6);
    cout << "hostname = " << hostname << endl;
}

void Request::set_connection_type(string& __token) {
    connection_type = __token.substr(12);
    cout << "connection_type = " << connection_type << endl;
}

void Request::set_user_agent(string& __token) {
    user_agent = __token.substr(12);
    cout << "user_agent = " << user_agent << endl;
}

void Request::set_accept(string& __token) {
    accept = __token.substr(8);
    cout << "accept = " << accept << endl;
}

void Request::set_referer(string& __token) {
    referer = __token.substr(9);
    cout << "referer = " << referer << endl;
}

bool Request::CheckHeader(string& token, string __header) {
    return token.find(__header) != string::npos;
}
