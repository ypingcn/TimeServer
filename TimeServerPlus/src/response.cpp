#include "response.h"

#include <iostream>
using std::cout;
using std::endl;

void tsp_response(int sockfd, tsp_request_t *request) {
  tsp_module_callback *match_callback = new tsp_module_callback;

  bool match = false;

  for (tsp_module_t i : tsp_module_get()) {
    if (tsp_response_match(request, &i) == 0) {
      match_callback = (&i.callback);
      match = true;
      break;
    }
  }

  if (match) {
    (*match_callback)((void *)(intptr_t)sockfd, (void *)request);
  } else {
    if (request->version != "HTTP/1.1") {
      tsp_response_http_version_not_supported(sockfd);
      tsp_response_server_name(sockfd);
      tsp_response_headers_end(sockfd);
    } else {
      tsp_response_not_implemented(sockfd);
      tsp_response_server_name(sockfd);
      tsp_response_headers_end(sockfd);
    }

    close(sockfd);
  }

  match_callback = nullptr;
  delete match_callback;
}

int tsp_response_match(const tsp_request_t *request,
                       const tsp_module_t *module) {
  if (!std::regex_match(request->url, module->pattern.url_pattern))
    return -1;
  if (module->pattern.method != "" && request->method != module->pattern.method)
    return -1;
  if (module->pattern.version == std::string("") ||
      module->pattern.version == std::string()) {
    if (request->version != std::string("HTTP/1.1"))
      return -1;
  }
  if (module->pattern.version != request->version)
    return -1;
  return 0;
}

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
  std::string name;
  if (tsp_config_get("SERVERSTRING", name) == -1)
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
