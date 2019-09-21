#include "include/HttpServer.h"

void HttpServer::OnConnect(uv_stream_t *server_handle, int status) {
    assert(status == 0);
    assert((uv_tcp_t*)server_handle == &server);

    auto *client = new uv_tcp_t;
    uv_tcp_init(uv_loop, client);

    if (uv_accept(server_handle, (uv_stream_t*) client)) {
        uv_close((uv_handle_t*) client, nullptr);
        return;
    }

    uv_read_start((uv_stream_t*) client, HttpServer::OnAlloc, HttpServer::OnRead);
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

      uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, HttpServer::OnConnect);
      cout << "server started listening on port " << DEFAULT_PORT << endl;
      return uv_run(uv_loop, UV_RUN_DEFAULT);
}
