//
// Created by jahongir on 9/18/19.
//

#include "../include/Request.h"

Request::Request() = default;

void Request::ParseHttpQuery(char* __str) {
    string __token;
    string str = string(__str);
    std::istringstream stream(str);
    std::getline(stream, __token, '\n');

    std::istringstream first_line(__token);
    first_line >> method >> uri >> http_version;
    if (method.empty() || uri.empty() || http_version.empty()) {
        return;
    }

    if (uri.find('%') != string::npos) {
        char ss[uri.length()];
        strcpy(ss, uri.c_str());

        char tmp[500];
        ParseUri(ss, tmp, uri.length());
        uri = tmp;
    }

    uri = uri.substr(0, uri.find('?'));
    http_version = http_version.substr(5);
    filename = "http-test-suite" + uri;

    cout << "method = " << method << endl;
    cout << "uri = " << uri << endl;
    cout << "filename = " << filename << endl;
    cout << "http_version = " << http_version << endl;

    while(std::getline(stream, __token, '\n')) {
        if (CheckHeader(__token, "Host")) {
            set_hostname(__token);
        } else if (CheckHeader(__token, "Connection")) {
            set_connection_type(__token);
        } else if (CheckHeader(__token, "User-Agent")) {
            set_user_agent(__token);
        } else if (CheckHeader(__token, "Accept:")) {
            set_accept(__token);
        } else if (CheckHeader(__token, "Referer")) {
            set_referer(__token);
        }
    }
}

void Request::ParseUri(char *src, char *dest, size_t len) {
    char *p = src;
    char code[3] = {0};
    while (*p != '\0') {
        if (*p == '%') {
            memcpy(code, ++p, 2);
            *dest++ = (char) strtoul(code, nullptr, 16);
            p += 2;
        } else {
            *dest++ = *p++;
        }
    }
    *dest = '\0';
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

bool Request::CheckHeader(const string& __token, const string& __header) {
    return __token.find(__header) != string::npos;
}
