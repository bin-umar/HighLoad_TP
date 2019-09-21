//
// Created by jahongir on 9/17/19.
//

#include "../include/Response.h"

Response::Response(int __fd): fd(__fd) {
    http_version = "1.1";
    content_length = 0;
    connection_type = "keep-alive";
    server = "Ubuntu(Pop OS)/19.02";
}

void Response::Status(int __st) {
    status = std::to_string(__st);
    phrase = status_phrase.at(__st);
}

void Response::SendHeaders(const string& __filename) {
    struct stat fileStat{};
    if (stat(__filename.c_str(), &fileStat) < 0) {
        std::cerr << "File " << __filename <<  " not found" << endl;
        return;
    }

    for (const auto& type: mime_types) {
        if (__filename.find(type.format) != std::string::npos) {
            content_type = type.mime;
        }
    }

    content_length = (size_t)fileStat.st_size;
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
    data += "Content-Type: " + content_type + "; charset=UTF-8" + CRLF + CRLF;
}
