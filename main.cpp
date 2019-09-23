#include "include/HttpServer.h"

#define DEFAULT_PORT  8000
#define DEFAULT_IP    "0.0.0.0"

int main() {
      new http_server::Server(DEFAULT_IP, DEFAULT_PORT);
      return 0;
}
