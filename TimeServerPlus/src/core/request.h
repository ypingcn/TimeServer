#ifndef TSP_REQUEST_H
#define TSP_REQUEST_H

#include "../contrib/logger.h"
#include "../include/type.h"
#include "response.h"

#include <sstream>
#include <unistd.h>

typedef map<string, string> TSPRequestArgsType;
typedef map<string, string> TSPRequestHeadersType;
#define new_header(key, value) std::make_pair((key), (value))
#define new_arg(key, value) std::make_pair((key), (value))

class TSPRequest {
public:
  explicit TSPRequest(int fd) : sockfd(fd){};
  ~TSPRequest() { close(sockfd); };
  TSPRequest(const TSPRequest &) = delete;
  TSPRequest &operator=(const TSPRequest &) = delete;

  int parse_from_string(const string &raw_request);

  string get_method() const { return method; }
  string get_url() const { return url; }
  string get_version() const { return version; }
  string get_body() const { return body; }
  int get_sockfd() const { return sockfd; }

  string get_args(const string &key) const;
  int exist_args(const string &key, const string &value) const;
  string get_headers(const string &key) const;
  int exist_headers(const string &key, const string &value) const;

  void debug() {
    for (auto i : args)
      cout << i.first << "#" << i.second << endl;
    for (auto i : headers)
      cout << i.first << "#" << i.second << endl;
  }

private:
  string method;
  string url;
  string version;
  TSPRequestArgsType args;
  TSPRequestHeadersType headers;
  string body;
  int sockfd;
};

int parse_args(const string &raw_string, TSPRequestArgsType &args);
int parse_headers(const string &raw_string, TSPRequestHeadersType &headers);

void tsp_request_handle(const TSPRequest &req);

#endif
