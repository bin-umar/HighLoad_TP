//
// Created by jahongir on 9/21/19.
//

#ifndef HIGHLOAD_TP_HTTPSERVER_H
#define HIGHLOAD_TP_HTTPSERVER_H

#include "Response.h"
#include "Request.h"

namespace http_server {
    static string document_root;

    void OnAlloc(uv_handle_t* client, size_t __suggested_size, uv_buf_t* __buf);
    void OnClose(uv_handle_t *__client);
    void OnRead(uv_stream_t *__client, ssize_t __n_read, const uv_buf_t *__buf);
    void OnConnect(uv_stream_t *server_handle, int status);

    class Server {
    public:
        Server(const string&, unsigned short, const string&, unsigned short);
        static void Worker(void* arg);
    };

    class HttpServer {
    public:
        explicit HttpServer(int __fd, const string& __root);
        HttpServer(const HttpServer&) = delete;
        HttpServer(HttpServer&&) = delete;
        ~HttpServer();

        HttpServer& operator=(const HttpServer&) = delete;
        HttpServer& operator=(HttpServer&&) = delete;

        void ParseRequest(char *);
        void SendResponse();

    private:
        Request* request;
        Response* response;
        int GetStatus();
    };
}

#endif //HIGHLOAD_TP_HTTPSERVER_H
