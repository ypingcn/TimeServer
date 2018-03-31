#include "module_time.h"
#include "../response.h"
#include "../utility.h"

#include <iostream>
#include <pthread.h>
using std::cout;
using std::endl;

void tsp_module_time_init(std::vector<tsp_module_t> &v) {
  tsp_module_t a = {{std::regex("^.*?localtime=1$"), std::string("GET"),
                     std::string("HTTP/1.1")},
                    tsp_module_time_localtime,
                    100};
  v.push_back(a);
  a = {
      {std::regex("^.*?ctime=1$"), std::string("GET"), std::string("HTTP/1.1")},
      tsp_module_time_file_ctime,
      100};
  v.push_back(a);
  a = {
      {std::regex("^.*?atime=1$"), std::string("GET"), std::string("HTTP/1.1")},
      tsp_module_time_file_atime,
      100};
  v.push_back(a);
  a = {
      {std::regex("^.*?mtime=1$"), std::string("GET"), std::string("HTTP/1.1")},
      tsp_module_time_file_mtime,
      100};
  v.push_back(a);
}

void *tsp_module_time_localtime(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;
  std::string file = client_request->url;

  tsp_response_ok(client_sockfd);
  tsp_response_headers_end(client_sockfd);

  char body[1024];
  struct tm *now;
  time_t timer = time(NULL);
  now = localtime(&timer);
  strftime(body, sizeof(body), "<p>Now: %F %X<p>\n", now);

  write(client_sockfd, body, strlen(body));
}

void *tsp_module_time_file_ctime(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;

  std::string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  std::string file = path + client_request->url;
  file = file.substr(0, file.find_first_of("?"));
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
    std::string res;
    TSPUtilFile::last_change(file.data(), res);
    res = "<p> " + res + " </p>";
    off_t size = 0;
    size = res.length();
    tsp_response_ok(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_content_length(size, client_sockfd);
    tsp_response_headers_end(client_sockfd);
    TSPUtilIO::basic_write(client_sockfd, const_cast<char *>(res.c_str()),
                           res.length());
  } else {
    tsp_response_not_found(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_headers_end(client_sockfd);
  }
  client_request = nullptr;
  delete client_request;
}

void *tsp_module_time_file_atime(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;

  std::string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  std::string file = path + client_request->url;
  file = file.substr(0, file.find_first_of("?"));
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
    std::string res;
    TSPUtilFile::last_access(file.data(), res);
    res = "<p> " + res + " </p>";
    off_t size = 0;
    size = res.length();
    tsp_response_ok(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_content_length(size, client_sockfd);
    tsp_response_headers_end(client_sockfd);
    TSPUtilIO::basic_write(client_sockfd, const_cast<char *>(res.c_str()),
                           res.length());
  } else {
    tsp_response_not_found(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_headers_end(client_sockfd);
  }
  client_request = nullptr;
  delete client_request;
}

void *tsp_module_time_file_mtime(void *sockfd, void *request) {
  int client_sockfd = (intptr_t)sockfd;
  tsp_request_t *client_request = (tsp_request_t *)request;

  std::string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  std::string file = path + client_request->url;
  file = file.substr(0, file.find_first_of("?"));
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
    std::string res;
    TSPUtilFile::last_modify(file.data(), res);
    res = "<p> " + res + " </p>";
    off_t size = 0;
    size = res.length();
    tsp_response_ok(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_content_length(size, client_sockfd);
    tsp_response_headers_end(client_sockfd);
    TSPUtilIO::basic_write(client_sockfd, const_cast<char *>(res.c_str()),
                           res.length());
  } else {
    tsp_response_not_found(client_sockfd);
    tsp_response_server_name(client_sockfd);
    tsp_response_headers_end(client_sockfd);
  }
  client_request = nullptr;
  delete client_request;
}
