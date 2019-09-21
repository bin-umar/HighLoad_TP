//
// Created by jahongir on 9/21/19.
//

#include "../include/HttpServer.h"

HttpServer::Server::Server(int __fd): fd(__fd) {
    response = new Response(__fd);
};

HttpServer::Server::~Server() {
    delete response;
}

void HttpServer::Server::ParseRequest(char * __request_string) {
    request.Parse(__request_string);
}

void HttpServer::Server::SendResponse() {
    int status = GetStatus();
    response->Status(status);
    response->SendHeaders(request.filename);

    if (status == HTTP_STATUS_OK) {
        response->SendFile(request.filename);
    }
}

int HttpServer::Server::GetStatus() {
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

void HttpServer::OnClose(uv_handle_t *__client) {
    cout << __client << "connection closed" << endl;
    delete __client;
    __client = nullptr;
}

void HttpServer::OnAlloc(uv_handle_t *client, size_t __suggested_size, uv_buf_t *__buf) {
    cout << "alloc_buffer " << __suggested_size << endl;
//    __buf->base = new char[__suggested_size]();
    __buf->base = (char*)malloc(__suggested_size);
    __buf->len = __suggested_size;
}

void HttpServer::OnRead(uv_stream_t *__client, ssize_t __n_read, const uv_buf_t *__buf) {
    cout << "echo_read(stream=" << __client->io_watcher.fd << "__n_read=" << __n_read << endl;
    if (__n_read < 0) {
        cout << "<EOF>" << endl;
        uv_close((uv_handle_t*) __client, nullptr);
//        delete[] __buf->base;
        free(__buf->base);
        return;
    }

    Server http_server(__client->io_watcher.fd);
    http_server.ParseRequest(__buf->base);
    http_server.SendResponse();

    uv_close((uv_handle_t*) __client, HttpServer::OnClose);
//    delete[] __buf->base;
    free(__buf->base);
}
