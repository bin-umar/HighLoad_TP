#include <sys/socket.h>
#include <iostream>
#include <cassert>
#include <cstring>
#include "uv.h"

static uv_tcp_t server;
static uv_loop_t* uv_loop;

#define DEFAULT_PORT  8000
#define DEFAULT_BACKLOG 1024

using std::cout;
using std::endl;

typedef struct {
    uv_tcp_t handle;
    uv_write_t write_req;
    int request_num;
} client_t;

void on_alloc(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf) {
    cout << "alloc_buffer " << suggested_size << endl;
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void on_close(uv_handle_t* handle) {
    auto* client = (client_t*) handle->data;
    cout << client << "connection closed" << endl;
    delete client;
}

void on_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    cout << "echo_read(stream=" << client->io_watcher.fd << "nread=" << nread << endl;
//    extern ssize_t send (int __fd, const void *__buf, size_t __n, int __flags);
    std::string hello = "HTTP/1.1 200 Ok \r\n";
    hello += "Server: MLQ/0.1.2\r\n";
    hello += "Connection: close\r\n";
    hello += "Date: Fri, 13 Sep 2019 00:57:02 GMT\r\n";
    hello += "Content-Length: 11\r\n";
    hello += "Content-Type: text/plain; charset=UTF-8\r\n\r\n";
    hello += "Hello world\n";

    if (nread < 0) {
        cout << "<EOF>" << endl;
        uv_close((uv_handle_t*) client, on_close);
    }

    int r = send(client->io_watcher.fd,  hello.c_str(), hello.length(), 0);
    cout << "sending " << r << endl;
    uv_close((uv_handle_t*) client, on_close);

    cout << buf->base << endl;
    delete[] buf->base;
}

void on_connect(uv_stream_t* server_handle, int status) {
    int r;

    assert(status == 0);
    assert((uv_tcp_t*)server_handle == &server);

    auto *client = new uv_tcp_t;
    uv_tcp_init(uv_loop, client);

    r = uv_accept(server_handle, (uv_stream_t*) client);
    if (r == 0) {
        uv_read_start((uv_stream_t*) client, on_alloc, on_read);
    } else {
        uv_close((uv_handle_t*) client, on_close);
    }
}

int main() {
      int r;

      uv_loop = uv_default_loop();
      r = uv_tcp_init(uv_loop, &server);
      assert(r == 0);

      struct sockaddr_in addr{};
      r = uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);
      assert(r == 0);

      r = uv_tcp_bind(&server, (const struct sockaddr*) &addr, 0);
      assert(r == 0);

      uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_connect);
      cout << "server started listening on port " << DEFAULT_PORT << endl;
      return uv_run(uv_loop, UV_RUN_DEFAULT);
}