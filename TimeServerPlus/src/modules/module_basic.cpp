#include "module_basic.h"

// curl 127.0.0.1:8668
int TSPBasicGetResponse::handle(const TSPRequest &req) {
  int client_sockfd = req.get_sockfd();

  string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  string file = path + req.get_url();
  if (req.get_url() == "/")
    if (TSPConfig::instance()->get("INDEX", index) != -1)
      file.append(index);

  if (TSPUtilFile::is_directory(file.data()) == 0) {
    string index;
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
      sendfile(client_sockfd, fd, &nwrite, size);
    }
    close(fd);
  } else {
    tsp_response_not_found(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_headers_end(client_sockfd);
  }
}

// curl 127.0.0.1:8668 -I
int TSPBasicHeadResponse::handle(const TSPRequest &req) {
  int client_sockfd = req.get_sockfd();

  string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  string file = path + req.get_url();
  if (req.get_url() == "/")
    if (TSPConfig::instance()->get("INDEX", index) != -1)
      file.append(index);

  if (TSPUtilFile::is_directory(file.data()) == 0) {
    string index;
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
    close(fd);
  } else {
    tsp_response_not_found(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_headers_end(client_sockfd);
  }
}

// curl -X OPTIONS 127.0.0.1:8668 -i
int TSPBasicOptionsResponse::handle(const TSPRequest &req) {
  int client_sockfd = req.get_sockfd();
  char allow[1024] = "Allow: OPTIONS, GET, HEAD\r\n";
  tsp_response_ok(client_sockfd);
  write(client_sockfd, allow, strlen(allow));
  tsp_response_date(client_sockfd);
  tsp_response_server_name(client_sockfd);
  tsp_response_content_length((off_t)0, client_sockfd);
  tsp_response_headers_end(client_sockfd);
}

// curl -X POST 127.0.0.1:8668 -i
int TSPBasicPostResponse::handle(const TSPRequest &req) {
  int client_sockfd = req.get_sockfd();

  tsp_response_not_implemented(client_sockfd);
  tsp_response_date(client_sockfd);
  tsp_response_server_name(client_sockfd);
  tsp_response_content_length((off_t)0, client_sockfd);
  tsp_response_headers_end(client_sockfd);
}

// curl -X PUT 127.0.0.1:8668 -i -d "body"
int TSPBasicPutResponse::handle(const TSPRequest &req) {
  int client_sockfd = req.get_sockfd();

  tsp_response_not_implemented(client_sockfd);
  tsp_response_date(client_sockfd);
  tsp_response_server_name(client_sockfd);
  tsp_response_content_length((off_t)0, client_sockfd);
  tsp_response_headers_end(client_sockfd);
}

// curl -X DELETE 127.0.0.1:8668 -i
int TSPBasicDeleteResponse::handle(const TSPRequest &req) {
  int client_sockfd = req.get_sockfd();

  tsp_response_not_implemented(client_sockfd);
  tsp_response_date(client_sockfd);
  tsp_response_server_name(client_sockfd);
  tsp_response_content_length((off_t)0, client_sockfd);
  tsp_response_headers_end(client_sockfd);
}

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
