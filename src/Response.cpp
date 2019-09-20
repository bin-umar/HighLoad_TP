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

void Response::Send(int __fd, const string& __filename) {
    struct stat fileStat{};
    if (stat(__filename.c_str(), &fileStat) < 0) {
        std::cerr << "File " << __filename <<  " not found" << endl;
        return;
    }

    for (const auto& type: mime_types) {
        if (__filename.find(type.format) != std::string::npos) {
            this->connection_type = type.mime;
        }
    }

    this->content_length = (size_t)fileStat.st_size;
    this->date = Response::getCurrentDateGMT();
    this->set_data();

    int r = send(__fd, this->data.c_str(), this->get_size(), 0);
    cout << "sending " << r << endl;
    this->SendFile(__fd, __filename);
}

void Response::SendFile(int __fd, const string& __filename) {
    int file = open(__filename.c_str(), O_RDONLY);
    int sentBytes = 0, remainData = this->content_length;
    off_t offset = 0;
    while ((sentBytes = sendfile(__fd, file, &offset, BUF_SIZE)) > 0) {
        remainData -= sentBytes;
    }

    if (remainData != 0) {
        std::cerr << "File " << __filename <<  " didn't send completely" << endl;
        return;
    }

    close(file);
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
}
