//
// Created by jahongir on 9/17/19.
//

#include "../include/Response.h"

Response::Response(int __fd): fd(__fd) {
    http_version = "1.1";
    content_length = 0;
    connection_type = "keep-alive";
    server = "Ubuntu(Pop OS)/19.02";
    content = "";
}

void Response::Status(int __st) {
    status = std::to_string(__st);
    phrase = status_phrase.at(__st);

    if (__st != HTTP_STATUS_OK) {
        content_type = "text/plain";
        content = status + " " + phrase;
        content_length = content.length();
    }
}

void Response::SendHeaders() {
    date = Response::getCurrentDateGMT();
    set_data();

    int r = send(fd, data.c_str(), get_size(), 0);
    cout << "sending " << r << endl;
}

void Response::SendFile(const string& __filename) {
    int file = open(__filename.c_str(), O_RDONLY);
    int sentBytes = 0, remainData = content_length;
    off_t offset = 0;
    while ((sentBytes = sendfile(fd, file, &offset, BUF_SIZE)) > 0) {
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
    return data.length();
}

void Response::set_data() {
    data =  "HTTP/" + http_version + " " + status + " " + phrase + CRLF;
    data += "Server: " + server + CRLF;
    data += "Connection: " + connection_type + CRLF;
    data += "Date: " + date + CRLF;
    data += "Content-Length: " + std::to_string(content_length) + CRLF;
    data += "Content-Type: " + content_type + CRLF + CRLF;
    data += content;
}
