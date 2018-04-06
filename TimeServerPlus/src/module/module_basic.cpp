#include "module_basic.h"
#include "../logger.h"

#include <cstring>

void tsp_module_basic_init(std::vector<tsp_module_t> &v) {
  tsp_module_t value = {{regex("^.*$"), string("GET"), string("HTTP/1.1")},
                        tsp_module_basic_get,
                        10};
  v.push_back(value);

  value = {{regex("^.*$"), string("HEAD"), string("HTTP/1.1")},
           tsp_module_basic_head,
           10};
  v.push_back(value);

  value = {{regex("^.*$"), string("OPTIONS"), string("HTTP/1.1")},
           tsp_module_basic_options,
           10};
  v.push_back(value);

  value = {{regex("^.*$"), string("PUT"), string("HTTP/1.1")},
           tsp_module_basic_put,
           10};
  v.push_back(value);

  value = {{regex("^.*$"), string("POST"), string("HTTP/1.1")},
           tsp_module_basic_post,
           10};
  v.push_back(value);

  value = {{regex("^.*$"), string("DELETE"), string("HTTP/1.1")},
           tsp_module_basic_delete,
           10};
  v.push_back(value);
}

void *tsp_module_basic_get(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;

  std::string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  std::string file = path + client_request->url;
  if (client_request->url == "/")
    if (TSPConfig::instance()->get("INDEX", index) != -1)
      file.append(index);

  if (TSPUtilFile::is_directory(file.data()) == 0) {
    std::string index;
    TSPConfig::instance()->get("INDEX", index);
    if (file[file.length() - 1] == '/')
      file.append(index);
    else
      file.append("/" + index);
  }

  if (TSPUtilFile::exist(file.data()) == 0) {
    int fd = open(file.data(), O_RDONLY);
    off_t size = 0;
    TSPUtilFile::size(file.data(), size);
    off_t nwrite = 0;
    tsp_response_ok(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_content_length(size, client_sockfd);
    tsp_response_headers_end(client_sockfd);
    sendfile(client_sockfd, fd, &nwrite, size);
    while (nwrite < size) {
      if (sendfile(client_sockfd, fd, &nwrite, size) < 0)
        TSPLogger::instance()->error("module basic get error [%s]",
                                     strerror(errno));
    }
  } else {
    tsp_response_not_found(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_headers_end(client_sockfd);
  }
  client_request = nullptr;
  delete client_request;
}

void *tsp_module_basic_head(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;

  std::string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  std::string file = path + client_request->url;
  if (client_request->url == "/")
    if (TSPConfig::instance()->get("INDEX", index) != -1)
      file.append(index);

  if (TSPUtilFile::is_directory(file.data()) == 0) {
    std::string index;
    TSPConfig::instance()->get("INDEX", index);
    if (file[file.length() - 1] == '/')
      file.append(index);
    else
      file.append("/" + index);
  }

  if (TSPUtilFile::exist(file.data()) == 0) {
    int fd = open(file.data(), O_RDONLY);
    off_t size = 0;
    TSPUtilFile::size(file.data(), size);
    tsp_response_ok(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_content_length(size, client_sockfd);
    tsp_response_headers_end(client_sockfd);
  } else {
    tsp_response_not_found(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_headers_end(client_sockfd);
  }
  client_request = nullptr;
  delete client_request;
}

// curl -X OPTIONS 127.0.0.1:8668 -i
void *tsp_module_basic_options(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;

  tsp_response_ok(client_sockfd);
  char allow[1024] = "Allow: OPTIONS, GET, HEAD\r\n";
  write(client_sockfd, allow, strlen(allow));
  tsp_response_date(client_sockfd);
  tsp_response_server_name(client_sockfd);
  tsp_response_content_length((off_t)0, client_sockfd);
  tsp_response_headers_end(client_sockfd);

  client_request = nullptr;
  delete client_request;
}

// curl -X POST 127.0.0.1:8668 -i
void *tsp_module_basic_post(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;

  tsp_response_not_implemented(client_sockfd);
  tsp_response_date(client_sockfd);
  tsp_response_server_name(client_sockfd);
  tsp_response_content_length((off_t)0, client_sockfd);
  tsp_response_headers_end(client_sockfd);
}

// curl -X PUT 127.0.0.1:8668 -i -d "body"
void *tsp_module_basic_put(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;

  tsp_response_not_implemented(client_sockfd);
  tsp_response_date(client_sockfd);
  tsp_response_server_name(client_sockfd);
  tsp_response_content_length((off_t)0, client_sockfd);
  tsp_response_headers_end(client_sockfd);
}

// curl -X DELETE 127.0.0.1:8668 -i
void *tsp_module_basic_delete(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;

  tsp_response_not_implemented(client_sockfd);
  tsp_response_date(client_sockfd);
  tsp_response_server_name(client_sockfd);
  tsp_response_content_length((off_t)0, client_sockfd);
  tsp_response_headers_end(client_sockfd);
}

int TSPBasicGetResponse::handle(const TSPRequest &req) {}
int TSPBasicHeadResponse::handle(const TSPRequest &req) {}
int TSPBasicOptionsResponse::handle(const TSPRequest &req) {}
int TSPBasicPostResponse::handle(const TSPRequest &req) {}
int TSPBasicPutResponse::handle(const TSPRequest &req) {}
int TSPBasicDeleteResponse::handle(const TSPRequest &req) {}

void install_basic_modules(ResponseVectorType &t) {
  t.push_back(new TSPBasicGetResponse(MIN_RESPONSE_PRIORITY,
                                      {regex("^.*$"), "GET", "HTTP/1.1"}));
  t.push_back(new TSPBasicHeadResponse(MIN_RESPONSE_PRIORITY,
                                       {regex("^.*$"), "HEAD", "HTTP/1.1"}));
  t.push_back(new TSPBasicOptionsResponse(
      MIN_RESPONSE_PRIORITY, {regex("^.*$"), "OPTIONS", "HTTP/1.1"}));
  t.push_back(new TSPBasicPostResponse(MIN_RESPONSE_PRIORITY,
                                       {regex("^.*$"), "POST", "HTTP/1.1"}));
  t.push_back(new TSPBasicPutResponse(MIN_RESPONSE_PRIORITY,
                                      {regex("^.*$"), "PUT", "HTTP/1.1"}));
  t.push_back(new TSPBasicDeleteResponse(
      MIN_RESPONSE_PRIORITY, {regex("^.*$"), "DELETE", "HTTP/1.1"}));
}
