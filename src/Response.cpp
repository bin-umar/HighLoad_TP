//
// Created by jahongir on 9/17/19.
//

#include "../include/Response.h"

Response::Response() {
    this->http_version = "1.1";
    this->content_length = 0;
    this->connection_type = "keep-alive";
    this->server = "Ubuntu(Pop OS)/19.02";
}

void Response::Status(int __st) {
    this->status = std::to_string(__st);
    this->phrase = status_phrase.at(__st);
}

void Response::Send(int __fd, const string& __data) {
    this->content_type = "text/plain";
    this->content = __data;
    this->content_length = __data.length();
    this->date = Response::getCurrentDateGMT();
    this->set_data();

    int r = send(__fd, this->data.c_str(), this->get_size(), 0);
    cout << "sending " << r << endl;
}

string Response::getCurrentDateGMT() {
    char buf[1000];
    time_t now = time(nullptr);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    return std::string(buf);
}

size_t Response::get_size() {
    return this->data.length();
}

void Response::set_data() {
    this->data = "HTTP/" + this->http_version + " " + this->status + " " + this->phrase + "\r\n";
    this->data += "Server: " + this->server + "\r\n";
    this->data += "Connection: " + this->connection_type + "\r\n";
    this->data += "Date: " + this->date + "\r\n";
    this->data += "Content-Length: " + std::to_string(this->content_length) + "\r\n";
    this->data += "Content-Type: " + this->content_type + "; charset=UTF-8\r\n\r\n";
    this->data += this->content;
}
