#include "include/Response.h"
#include "include/Request.h"

class Server {
public:
    Server();
    static void OnConnect(uv_stream_t* server_handle, int status);

private:
    static void OnRead(uv_stream_t* __client, ssize_t __n_read, const uv_buf_t* __buf);
    static void OnAlloc(uv_handle_t* __client, size_t __suggested_size, uv_buf_t* __buf);
    static void OnClose(uv_handle_t* __client);
};

Server::Server() = default;

void Server::OnAlloc(uv_handle_t* client, size_t __suggested_size, uv_buf_t* __buf) {
    cout << "alloc_buffer " << __suggested_size << endl;
    __buf->base = new char[__suggested_size];
    __buf->len = __suggested_size;
}

void Server::OnClose(uv_handle_t *__client) {
    cout << __client << "connection closed" << endl;
    delete __client;
}

void Server::OnRead(uv_stream_t *__client, ssize_t __n_read, const uv_buf_t *__buf) {
    cout << "echo_read(stream=" << __client->io_watcher.fd << "__n_read=" << __n_read << endl;

    if (__n_read < 0) {
        cout << "<EOF>" << endl;
        uv_close((uv_handle_t*) __client, Server::OnClose);
        return;
    }

    Request request;
    request.Parse(__buf->base);

    Response response;
    response.Status(200);
    response.Send(__client->io_watcher.fd, "index.html");

    uv_close((uv_handle_t*) __client, Server::OnClose);
    delete[] __buf->base;
}

void Server::OnConnect(uv_stream_t *server_handle, int status) {
    int r;

    assert(status == 0);
    assert((uv_tcp_t*)server_handle == &server);

    auto *client = new uv_tcp_t;
    uv_tcp_init(uv_loop, client);

    r = uv_accept(server_handle, (uv_stream_t*) client);
    if (r == 0) {
        uv_read_start((uv_stream_t*) client, Server::OnAlloc, Server::OnRead);
    } else {
        uv_close((uv_handle_t*) client, Server::OnClose);
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

      uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, Server::OnConnect);
      cout << "server started listening on port " << DEFAULT_PORT << endl;
      return uv_run(uv_loop, UV_RUN_DEFAULT);
}
