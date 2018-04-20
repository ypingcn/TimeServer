#include "module_time.h"

int TSPTimeLocaltimeResponse::handle(const TSPRequest &req) {
  int sockfd = req.get_sockfd();
  string file = req.get_url();

  tsp_response_ok(sockfd);
  tsp_response_headers_end(sockfd);

  char body[1024];
  struct tm *now;
  time_t timer = time(NULL);
  now = localtime(&timer);
  strftime(body, sizeof(body), "<p>Now: %F %X<p>\n", now);

  write(sockfd, body, strlen(body));
}

int TSPTimeFileCtimeResponse::handle(const TSPRequest &req) {
  int client_sockfd = req.get_sockfd();

  string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  string file = path + req.get_url();
  file = file.substr(0, file.find_first_of("?"));
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
    string res;
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
}

int TSPTimeFileAtimeResponse::handle(const TSPRequest &req) {
  int client_sockfd = req.get_sockfd();

  string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  string file = path + req.get_url();
  file = file.substr(0, file.find_first_of("?"));
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
    string res;
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
}

int TSPTimeFileMtimeResponse::handle(const TSPRequest &req) {
  int client_sockfd = req.get_sockfd();

  string path, index;
  TSPConfig::instance()->get("DOCROOT", path);
  string file = path + req.get_url();
  file = file.substr(0, file.find_first_of("?"));
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
    string res;
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
}

void install_time_modules(ResponseVectorType &t) {
  t.push_back(new TSPTimeFileCtimeResponse(
      DEFAULT_RESPONSE_PRIORITY, {regex("^.*?ctime=1$"), "GET", "HTTP/1.1"}));
  t.push_back(new TSPTimeFileAtimeResponse(
      DEFAULT_RESPONSE_PRIORITY, {regex("^.*?atime=1$"), "GET", "HTTP/1.1"}));
  t.push_back(new TSPTimeFileMtimeResponse(
      DEFAULT_RESPONSE_PRIORITY, {regex("^.*?mtime=1$"), "GET", "HTTP/1.1"}));
  t.push_back(new TSPTimeLocaltimeResponse(
      DEFAULT_RESPONSE_PRIORITY,
      {regex("^.*?localtime=1$"), "GET", "HTTP/1.1"}));
}
