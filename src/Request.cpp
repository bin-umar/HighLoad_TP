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
    first_line >> this->method >> this->uri >> this->http_version;
    this->http_version = this->http_version.substr(5);

    cout << "method = " << this->method << endl;
    cout << "uri = " << this->uri << endl;
    cout << "http_version = " << this->http_version << endl;

    while(std::getline(stream, token, '\n')) {
        if (this->CheckHeader(token, "Host")) {
            this->set_hostname(token);
        } else if (this->CheckHeader(token, "Connection")) {
            this->set_connection_type(token);
        } else if (this->CheckHeader(token, "User-Agent")) {
            this->set_user_agent(token);
        } else if (this->CheckHeader(token, "Accept:")) {
            this->set_accept(token);
        } else if (this->CheckHeader(token, "Referer")) {
            this->set_referer(token);
        }
    }
}

void Request::set_hostname(string& __token) {
    this->hostname = __token.substr(6);
    cout << "hostname = " << this->hostname << endl;
}

void Request::set_connection_type(string& __token) {
    this->connection_type = __token.substr(12);
    cout << "connection_type = " << this->connection_type << endl;
}

void Request::set_user_agent(string& __token) {
    this->user_agent = __token.substr(12);
    cout << "user_agent = " << this->user_agent << endl;
}

void Request::set_accept(string& __token) {
    this->accept = __token.substr(8);
    cout << "accept = " << this->accept << endl;
}

void Request::set_referer(string& __token) {
    this->referer = __token.substr(9);
    cout << "referer = " << this->referer << endl;
}

bool Request::CheckHeader(string& token, string __header) {
    return token.find(__header) != string::npos;
}
