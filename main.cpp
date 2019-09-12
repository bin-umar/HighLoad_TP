#include <iostream>
#include <cassert>
#include "libuv/include/uv.h"
#include "http-parser/http_parser.h"

static uv_tcp_t server;
static uv_loop_t* uv_loop;
static http_parser_settings parser_settings;

using std::cout;
using std::endl;

typedef struct {
    uv_tcp_t handle;
    http_parser parser;
    uv_write_t write_req;
    int request_num;
} client_t;

void on_alloc(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf) {
    buf->base = new char(suggested_size);
    buf->len = suggested_size;
}

void on_close(uv_handle_t* handle) {
    auto* client = (client_t*) handle->data;
    cout << client->request_num << "connection closed" << endl;
    delete client;
}

void on_read(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf) {
    size_t parsed;
    auto client = (client_t*) tcp->data;

    if (nread >= 0) {
        parsed = http_parser_execute(&client->parser, &parser_settings, buf->base, nread);
        if (parsed < nread) {
            std::cerr << "parse error" << endl;
            uv_close((uv_handle_t*) &client->handle, on_close);
        }
    }

    delete buf->base;
}

static int request_num = 1;

void on_connect(uv_stream_t* server_handle, int status) {
    assert(status == 0);
    int r;
    assert((uv_tcp_t*)server_handle == &server);

    auto *client = new client_t;
    client->request_num = request_num;
    cout << request_num++ << "new connection" << endl;

    uv_tcp_init(uv_loop, &client->handle);
    http_parser_init(&client->parser, HTTP_REQUEST);

    client->parser.data = client;
    client->handle.data = client;

    r = uv_accept(server_handle, (uv_stream_t*)&client->handle);
    assert(r == 0);

    uv_read_start((uv_stream_t*)&client->handle, on_alloc, on_read);
}

int main() {
      int r;

      uv_loop = uv_default_loop();
      r = uv_tcp_init(uv_loop, &server);
      assert(r == 0);

      struct sockaddr_in addr{};
      r = uv_ip4_addr("0.0.0.0", 8000, &addr);
      assert(r == 0);

      r = uv_tcp_bind(&server, (const struct sockaddr*) &addr, 0);
      assert(r == 0);

      uv_listen((uv_stream_t*) &server, 128, on_connect);
      cout << "listening on port 8000" << endl;
      return uv_run(uv_loop, UV_RUN_DEFAULT);
}
