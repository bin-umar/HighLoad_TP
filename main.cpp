#include <iostream>
#include "libuv/include/uv.h"

static uv_tcp_t server;

int main() {
    uv_init();
    uv_tcp_init(server, NULL);
    std::cout<<"Hello, World!"<<std::endl;

    uv_run();
    return 0;

}
