//
// Created by jahongir on 9/21/19.
//

#ifndef HIGHLOAD_TP_HTTPSERVER_H
#define HIGHLOAD_TP_HTTPSERVER_H

#include "Response.h"
#include "Request.h"

namespace HttpServer {
    void OnAlloc(uv_handle_t* client, size_t __suggested_size, uv_buf_t* __buf);
    void OnClose(uv_handle_t *__client);
    void OnRead(uv_stream_t *__client, ssize_t __n_read, const uv_buf_t *__buf);
    void OnConnect(uv_stream_t *server_handle, int status);

    class Server {
    public:
        explicit Server(int __fd);
        void ParseRequest(char *);
        void SendResponse();
        ~Server();

    private:
        Request request;
        Response* response;
        int fd;

        int GetStatus();
    };
}

#endif //HIGHLOAD_TP_HTTPSERVER_H
