#include <cassert>
#include <sstream>

#include "include/Status.h"
#include "include/Response.h"
#include "include/Request.h"

static uv_tcp_t server;
static uv_loop_t* uv_loop;

#define DEFAULT_PORT  8000
#define DEFAULT_BACKLOG 1024


void on_alloc(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf) {
    cout << "alloc_buffer " << suggested_size << endl;
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void on_close(uv_handle_t* client) {
    cout << client << "connection closed" << endl;
    delete client;
}

void on_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    cout << "echo_read(stream=" << client->io_watcher.fd << "nread=" << nread << endl;

    if (nread < 0) {
        cout << "<EOF>" << endl;
        uv_close((uv_handle_t*) client, on_close);
        return;
    }

//    cout << buf->base << endl;
    Request request;
    request.Parse(buf->base);

    Response response;
    response.Status(200);
    response.Send(client->io_watcher.fd, "Hello world\n");
    uv_close((uv_handle_t*) client, on_close);
//    char* token = strtok(buf->base, "\n");
//    while(token != nullptr) {
//        cout << "token = " << token << endl;
//        token = strtok(nullptr, "\n");
//    }

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
