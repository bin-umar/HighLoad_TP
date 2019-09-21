//
// Created by jahongir on 9/21/19.
//

#include "../include/HttpServer.h"

http_server::HttpServer::HttpServer(int __fd): fd(__fd) {
    response = new Response(__fd);
};

http_server::HttpServer::~HttpServer() {
    delete response;
}

void http_server::HttpServer::ParseRequest(char * __request_string) {
    request.Parse(__request_string);
}

void http_server::HttpServer::SendResponse() {
    int status = GetStatus();
    response->Status(status);
    response->SendHeaders(request.filename);

    if (status == HTTP_STATUS_OK) {
        response->SendFile(request.filename);
    }
}

int http_server::HttpServer::GetStatus() {
    if (request.method != "GET" && request.method != "HEAD") {
        return HTTP_STATUS_METHOD_NOT_ALLOWED;
    }

    if (request.uri[request.uri.length()-1] == '/') {
        if (request.uri.find('.') == std::string::npos) {
            request.filename += "index.html";
            struct stat isExist{};
            if (stat(request.filename.c_str(), &isExist) < 0) {
                return HTTP_STATUS_FORBIDDEN;
            }
        }
    }

    return HTTP_STATUS_OK;
}

void http_server::OnClose(uv_handle_t *__client) {
    cout << __client << "connection closed" << endl;
    delete __client;
    __client = nullptr;
}

void http_server::OnAlloc(uv_handle_t *client, size_t __suggested_size, uv_buf_t *__buf) {
    cout << "alloc_buffer " << __suggested_size << endl;
    __buf->base = (char*)malloc(__suggested_size);  //    __buf->base = new char[__suggested_size]();
    __buf->len = __suggested_size;
}

void http_server::OnRead(uv_stream_t *__client, ssize_t __n_read, const uv_buf_t *__buf) {
    cout << "echo_read(stream=" << __client->io_watcher.fd << "__n_read=" << __n_read << endl;
    if (__n_read < 0) {
        cout << "<EOF>" << endl;
        uv_close((uv_handle_t*) __client, nullptr);
        free(__buf->base);
        return;
    }

    HttpServer http_server(__client->io_watcher.fd);
    http_server.ParseRequest(__buf->base);
    http_server.SendResponse();

    uv_close((uv_handle_t*) __client, http_server::OnClose);
    free(__buf->base); //    delete[] __buf->base;
}

void http_server::OnConnect(uv_stream_t *server_handle, int status) {
    assert(status == 0);
    assert((uv_tcp_t*)server_handle == &server);

    auto *client = new uv_tcp_t;
    uv_tcp_init(uv_loop, client);

    if (uv_accept(server_handle, (uv_stream_t*) client)) {
        uv_close((uv_handle_t*) client, nullptr);
        return;
    }

    uv_read_start((uv_stream_t*) client, http_server::OnAlloc, http_server::OnRead);
}

http_server::Server::Server(const string& ip, unsigned short port) {
    uv_loop = uv_default_loop();
    assert(uv_tcp_init(uv_loop, &server) == 0);

    struct sockaddr_in addr{};
    assert(uv_ip4_addr(ip.c_str(), port, &addr) == 0);
    assert(uv_tcp_bind(&server, (const struct sockaddr*) &addr, 0) == 0);

    uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, http_server::OnConnect);
    cout << "server started listening on port " << DEFAULT_PORT << endl;
    uv_run(uv_loop, UV_RUN_DEFAULT);
}
