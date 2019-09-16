#include <sys/socket.h>
#include <iostream>
#include <cassert>
#include <cstring>
#include <ctime>
#include "Status.h"
#include "uv.h"

static uv_tcp_t server;
static uv_loop_t* uv_loop;

#define DEFAULT_PORT  8000
#define DEFAULT_BACKLOG 1024

using std::cout;
using std::endl;
using std::string;

struct Request {
    string data;
    string method;
    string uri;
    string version;
    string filename;
};

class Response {
public:
    Response() {
        this->http_version = "1.1";
        this->content_length = 0;
        this->connection_type = "keep-alive";
        this->server = "Ubuntu(Pop OS)/19.02";
    };

    void Status(int __st) {
        this->status = std::to_string(__st);
        this->phrase = status_phrase.at(__st);
    };

    void Send(int __fd, const string& __data) {
        this->content_type = "text/plain";
        this->content = __data;
        this->content_length = __data.length();
        this->date = Response::getCurrentDateGMT();
        this->set_data();

        int r = send(__fd, this->data.c_str(), this->get_size(), 0);
        cout << "sending " << r << endl;
    };

private:
    string data;
    string http_version;
    string content_type;
    string date;
    string server;
    string connection_type;
    string content;
    string status;
    string phrase;
    size_t content_length;

    static string getCurrentDateGMT() {
        char buf[1000];
        time_t now = time(nullptr);
        struct tm tm = *gmtime(&now);
        strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
        return std::string(buf);
    };

    size_t get_size() {
        return this->data.length();
    };

    void set_data() {
        this->data = "HTTP/" + this->http_version + " " + this->status + " " + this->phrase + "\r\n";
        this->data += "Server: " + this->server + "\r\n";
        this->data += "Connection: " + this->connection_type + "\r\n";
        this->data += "Date: " + this->date + "\r\n";
        this->data += "Content-Length: " + std::to_string(this->content_length) + "\r\n";
        this->data += "Content-Type: " + this->content_type + "; charset=UTF-8\r\n\r\n";
        this->data += this->content;
    };
};

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
    }

    Response response;
    response.Status(200);
    response.Send(client->io_watcher.fd, "Hello world\n");
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
