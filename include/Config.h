//
// Created by jahongir on 9/18/19.
//

#ifndef HIGHLOAD_TP_CONFIG_H
#define HIGHLOAD_TP_CONFIG_H

#include <iostream>
#include <ctime>
#include <cstring>
#include <vector>
#include <cassert>
#include <sstream>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include "uv.h"

using std::cout;
using std::endl;
using std::string;

#define DEFAULT_BACKLOG 1024
#define BUF_SIZE 32
#define CRLF "\r\n"

struct types {
    const std::string format;
    const std::string mime;
};

const std::vector<types> mime_types = {
    { ".html", "text/html" },
    { ".css", "text/css" },
    { ".js", "application/javascript" },
    { ".jpg", "image/jpeg" },
    { ".jpeg", "image/jpeg" },
    { ".png", "image/png" },
    { ".gif", "image/gif" },
    { ".swf", "application/x-shockwave-flash" },
    { ".txt", "text/plain" },
};

#endif //HIGHLOAD_TP_CONFIG_H
