#include "include/Settings.h"
#include "include/HttpServer.h"


int main() {
    Settings settings;
    settings.ParseConfigFile("etc/httpd.conf");
    new http_server::Server(DEFAULT_IP, DEFAULT_PORT, settings.document_root, settings.cpu_limit);
    return 0;
}
