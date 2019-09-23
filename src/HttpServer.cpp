//
// Created by jahongir on 9/21/19.
//

#include "../include/HttpServer.h"

/****************************** Http Server ***********************************/
http_server::HttpServer::HttpServer(int __fd, const string& __root) {
    request = new Request(__root);
    response = new Response(__fd);
}

http_server::HttpServer::~HttpServer() {
    delete request;
    delete response;
}

void http_server::HttpServer::ParseRequest(char * __request_string) {
    request->ParseHttpQuery(__request_string);
}

void http_server::HttpServer::SendResponse() {
    int status = GetStatus();
    response->Status(status);
    response->SendHeaders();

    if (status == HTTP_STATUS_OK && request->method == "GET") {
        response->SendFile(request->filename);
    }
}

int http_server::HttpServer::GetStatus() {
    if (request->method.empty() || request->uri.empty() || request->http_version.empty()) {
        return HTTP_STATUS_METHOD_NOT_ALLOWED;
    }

    if (request->method != "GET" && request->method != "HEAD") {
        return HTTP_STATUS_METHOD_NOT_ALLOWED;
    }

    struct stat fileStat{};
    if (request->uri[request->uri.length() - 1] == '/') {
        if (request->uri.find('.') == string::npos) {
            request->filename += "index.html";
            if (stat(request->filename.c_str(), &fileStat) < 0) {
                return HTTP_STATUS_FORBIDDEN;
            }
        }
    }

    if (request->filename.find("/..") != string::npos) {
        return HTTP_STATUS_FORBIDDEN;
    }

    if (stat(request->filename.c_str(), &fileStat) < 0) {
        std::cerr << "File " << request->filename <<  " not found" << endl;
        return HTTP_STATUS_NOT_FOUND;
    }

    for (const auto& type: mime_types) {
        if (request->filename.find(type.format) != string::npos) {
            response->content_type = type.mime;
            response->content_length = (size_t)fileStat.st_size;
        }
    }

    return HTTP_STATUS_OK;
}

/************************************* Server *************************************/
http_server::Server::Server(const string& ip, unsigned short port) {
    struct sockaddr_in address{};
    uv_ip4_addr(ip.c_str(), port, &address);
    cout << "server started listening on port " << port << endl;
    for (int i = 0; i < 1; i++) {
        auto *worker = new uv_thread_t; // (uv_thread_t*)malloc(sizeof(uv_thread_t));
        uv_thread_create(worker, Server::Worker, &address);
        workers.push_back(worker);
    }

    for (auto worker : workers) {
        uv_thread_join(worker);
    }
}

void http_server::Server::Worker(void *arg) {
    uv_tcp_t server;
    auto* loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));  // auto* loop = new uv_loop_t;
    uv_loop_init(loop);

    assert(uv_tcp_init(loop, &server) == 0);
    assert(uv_tcp_bind(&server, (const struct sockaddr*) arg, 0) == 0);

    uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, http_server::OnConnect);
    uv_run(loop, UV_RUN_DEFAULT);
}

/***************************** CallBack Functions **********************************/
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
    cout << "echo_read(stream = " << __client->io_watcher.fd << "__n_read=" << __n_read << endl;
    if (__n_read < 0) {
        cout << "<EOF>" << endl;
        uv_close((uv_handle_t*) __client, nullptr);
        free(__buf->base);
        return;
    }

    HttpServer http_server(__client->io_watcher.fd, "http-test-suite");
    http_server.ParseRequest(__buf->base);
    http_server.SendResponse();

    uv_close((uv_handle_t*) __client, http_server::OnClose);
    free(__buf->base); //    delete[] __buf->base;
}

void http_server::OnConnect(uv_stream_t *server_handle, int status) {
    assert(status == 0);

    auto *client = new uv_tcp_t;
    uv_tcp_init(server_handle->loop, client);

    if (uv_accept(server_handle, (uv_stream_t*) client)) {
        uv_close((uv_handle_t*) client, nullptr);
        return;
    }

    uv_read_start((uv_stream_t*) client, http_server::OnAlloc, http_server::OnRead);
}
