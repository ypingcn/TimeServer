#include "response.h"

void tsp_response_headers_end(int sockfd) {
  char status[10] = TSP_RN;
  write(sockfd, status, strlen(status));
}

void tsp_response_date(int sockfd) {
  time_t now = time(nullptr);
  struct tm *gmt = gmtime(&now);

  string wdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  string mons[] = {"Jan",  "Feb", "Mar",  "Apr", "May", "June",
                   "July", "Aug", "Sept", "Oct", "Nov", "Dec"};

  char status[1024];

  snprintf(status, sizeof(status), "Date: %s, %d %s %4d %02d:%02d:%02d GMT\r\n",
           wdays[gmt->tm_wday].data(), gmt->tm_mday, mons[gmt->tm_mon].data(),
           gmt->tm_year + 1900, gmt->tm_hour, gmt->tm_min, gmt->tm_sec);

  write(sockfd, status, strlen(status));
}

void tsp_response_expires(int sockfd) {}

void tsp_response_server_name(int sockfd) {
  string name;
  if (TSPConfig::instance()->get("SERVERSTRING", name) == -1)
    name = "TimeServerPlus(TSP)/0.1.171007(cpp)";
  char status[1024];
  snprintf(status, sizeof(status), "Server: %s\r\n", name.data());
  write(sockfd, status, strlen(status));
}

void tsp_response_content_length(off_t size, int sockfd) {
  char status[1024];
  snprintf(status, sizeof(status), "Content-Length: %d\r\n", (int)size);
  write(sockfd, status, strlen(status));
}

void tsp_response_ok(int sockfd) {
  char status[1024];
  snprintf(status, sizeof(status), "HTTP/1.1 %d %s\r\n", TSP_RESPONSE_OK, "OK");
  write(sockfd, status, strlen(status));
}

void tsp_response_bad_request(int sockfd) {
  char status[1024];
  snprintf(status, sizeof(status), "HTTP/1.1 %d %s\r\n",
           TSP_RESPONSE_BAD_REQUEST, "Bad Request");
  printf("%s\n", status);
  write(sockfd, status, strlen(status));
}

void tsp_response_forbidden(int sockfd) {
  char status[1024];
  snprintf(status, sizeof(status), "HTTP/1.1 %d %s\r\n", TSP_RESPONSE_FORBIDDEN,
           "Forbidden");
  write(sockfd, status, strlen(status));
}

void tsp_response_not_found(int sockfd) {
  char status[1024];
  snprintf(status, sizeof(status), "HTTP/1.1 %d %s\r\n", TSP_RESPONSE_NOT_FOUND,
           "Not Found");
  write(sockfd, status, strlen(status));
}

void tsp_response_internal_server_error(int sockfd) {
  char status[1024];
  snprintf(status, sizeof(status), "HTTP/1.1 %d %s\r\n",
           TSP_RESPONSE_INTERNAL_SERVER_ERROR, "Internal Server Error");
  write(sockfd, status, strlen(status));
}

void tsp_response_not_implemented(int sockfd) {
  char status[1024];
  snprintf(status, sizeof(status), "HTTP/1.1 %d %s\r\n",
           TSP_RESPONSE_NOIMPLEMENTED, "Not implemented");
  write(sockfd, status, strlen(status));
}

void tsp_response_http_version_not_supported(int sockfd) {
  char status[1024];
  snprintf(status, sizeof(status), "HTTP/1.1 %d %s\r\n",
           TSP_RESPONSE_HTTP_VERSION_NOT_SUPPORTED,
           "Http Version Not Supported");
  write(sockfd, status, strlen(status));
}

int TSPBasicResponse::match(const TSPRequest &req) {
  string url = req.get_url();
  string method = req.get_method();
  string version = req.get_version();

  if (!std::regex_match(url, this->pattern.url_pattern))
    return -1;
  if (this->pattern.method != "" && method != this->pattern.method)
    return -1;
  if (this->pattern.version == string("") ||
      this->pattern.version == string()) {
    if (version != string("HTTP/1.1"))
      return -1;
  }
  if (this->pattern.version != version)
    return -1;
  return 0;
}

void tsp_response(const TSPRequest &req) {
  bool match = false;

  for (TSPBasicResponse *i : TSPModuleManager::get()) {
    if (i->match(req) == 0) {
      i->handle(req);
      match = true;
      break;
    }
  }

  if (!match) {
    int sockfd = req.get_sockfd();
    if (req.get_version() != "HTTP/1.1") {
      tsp_response_http_version_not_supported(sockfd);
      tsp_response_server_name(sockfd);
      tsp_response_headers_end(sockfd);
    } else {
      tsp_response_not_implemented(sockfd);
      tsp_response_server_name(sockfd);
      tsp_response_headers_end(sockfd);
    }
  }
}
