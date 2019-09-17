//
// Created by jahongir on 9/18/19.
//

#ifndef HIGHLOAD_TP_CONFIG_H
#define HIGHLOAD_TP_CONFIG_H

#include <iostream>
#include <ctime>
#include <cstring>
#include <cassert>
#include <sstream>
#include <sys/socket.h>
#include "uv.h"

using std::cout;
using std::endl;
using std::string;

static uv_tcp_t server;
static uv_loop_t* uv_loop;

#define DEFAULT_PORT  8000
#define DEFAULT_BACKLOG 1024

#endif //HIGHLOAD_TP_CONFIG_H
