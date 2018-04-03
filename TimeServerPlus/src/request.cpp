#include "request.h"
#include "logger.h"

#include <cstring>

tsp_request_t *tsp_request_new(void) {
  tsp_request_t *res = (tsp_request_t *)new tsp_request_t;
  if (res == NULL) {
    TSPLogger::instance()->error("request_t new error");
    exit(-1);
  }
  return res;
}

int tsp_request_delete(tsp_request_t *header) {
  if (header == NULL)
    return -1;
  delete header;
  return 0;
}

using std::cout;
using std::endl;

int tsp_request_parse(const std::string &raw_request,
                      tsp_request_t *parse_result) {
  if (raw_request.empty()) {
    TSPLogger::instance()->error("headers parse error empty raw_request [%s]",
                                 strerror(errno));
    return -1;
  }
  if (parse_result == NULL) {
    TSPLogger::instance()->error("headers parse error empty result [%s]",
                                 strerror(errno));
    return -1;
  }

  std::string rn("\r\n"), rnrn("\r\n\r\n");
  size_t pre = 0, nxt = 0, rnsize = 2;

  if ((nxt = raw_request.find(rn, pre)) != std::string::npos) {
    std::string line(raw_request.substr(pre, nxt - pre));
    cout << line << endl;
    std::stringstream s(line);
    s >> parse_result->method;
    s >> parse_result->url;
    s >> parse_result->version;

    if (parse_result->version.find("HTTP", 0) == std::string::npos)
      return -1;

    std::string url_line(parse_result->url);
    size_t arg_begin = url_line.find('?', 0);

    if (arg_begin != std::string::npos) {
      arg_begin++;
      size_t i = arg_begin, j = arg_begin;
      std::string key, value;

      size_t arg_next = url_line.find("&&", arg_begin);
      while (arg_next != std::string::npos) {
        while (url_line[j] != '=')
          j++;

        key = url_line.substr(i, j - i);

        j++;
        i = j;

        j = arg_next;

        value = url_line.substr(i, j - i);

        i = j = arg_next + 2;
        arg_next = url_line.find("&&", i);
      }

      while (url_line[j] != '=' && j < url_line.size())
        j++;

      if (j == url_line.size())
        return -1;

      key = url_line.substr(i, j - i);

      j++;
      i = j;

      j = url_line.size();

      value = url_line.substr(i, j - i);
    }

    pre = nxt;
  } else {
    TSPLogger::instance()->error(
        "headers parse error raw_request without '\r\n' [%s]", strerror(errno));
    return -1;
  }

  size_t headers_end_pos = 0;

  if ((headers_end_pos = raw_request.find(rnrn, pre)) != std::string::npos) {
    std::string line, key, value;
    nxt = raw_request.find(rn, pre + rnsize);

    size_t i, j;

    while (nxt <= headers_end_pos) {
      line = raw_request.substr(pre + rnsize, nxt - pre - rnsize);
      i = 0, j = 0;

      while (isblank(line[j]))
        j++;
      i = j;

      while (!isblank(line[j]) && line[j] != ':')
        j++;

      key = line.substr(i, j - i);

      while (!isblank(line[j]))
        j++;

      i = j;

      while (j != nxt)
        j++;

      value = line.substr(i, j - i);

      parse_result->headers.insert(new_httpheader(key, value));

      pre = nxt;
      nxt = raw_request.find(rn, pre + rnsize);
    }
  }

  parse_result->body = raw_request.substr(headers_end_pos + rnsize + rnsize,
                                          raw_request.size() - rnsize - rnsize);

  return 0;
}

using std::cout;
using std::endl;

void tsp_request_handle(int sockfd, tsp_request_t *request) {
  tsp_response(sockfd, request);
}

int TSPRequest::parse_from_string(const string &raw_request) {
  if (raw_request.empty()) {
    TSPLogger::instance()->error("headers parse error empty raw_request [%s]",
                                 strerror(errno));
    return -1;
  }

  string rn("\r\n"), rnrn("\r\n\r\n");

  string method, url, version, body;
  TSPRequestArgsType args;
  TSPRequestHeadersType headers;

  size_t pre = 0, nxt = 0, rnsize = 2;

  if ((nxt = raw_request.find(rn, pre)) != std::string::npos) {
    string line(raw_request.substr(pre, nxt - pre));
    cout << line << endl;
    std::stringstream s(line);
    s >> method;
    s >> url;
    s >> version;

    if (version.find("HTTP", 0) == std::string::npos)
      return -1;

    string url_line(url);
    if (parse_args(url_line, args) == -1)
      return -1;

    pre = nxt;
  } else {
    TSPLogger::instance()->error(
        "headers parse error raw_request without '\r\n' [%s]", strerror(errno));
    return -1;
  }

  size_t headers_end_pos = 0;

  if ((headers_end_pos = raw_request.find(rnrn, pre)) != string::npos) {
    string header_line;
    nxt = raw_request.find(rn, pre + rnsize);
    while (nxt <= headers_end_pos) {
      header_line = raw_request.substr(pre + rnsize, nxt - pre - rnsize);
      if (parse_headers(header_line, headers) == -1)
        return -1;
      pre = nxt;
      nxt = raw_request.find(rn, pre + rnsize);
    }
  }

  body = raw_request.substr(headers_end_pos + rnsize + rnsize,
                            raw_request.size() - rnsize - rnsize);

  this->method = method;
  this->url = url;
  this->version = version;
  this->body = body;

  this->args = args;
  this->headers = headers;

  return 0;
}

int parse_args(const string &raw_string, TSPRequestArgsType &args) {
  if (raw_string.empty())
    return -1;
  size_t begin = raw_string.find('?', 0);

  if (begin != string::npos) {
    begin++;
    size_t i = begin, j = begin;
    string key, value;

    size_t next = raw_string.find("&&", begin);
    while (next != std::string::npos) {
      while (raw_string[j] != '=')
        j++;

      key = raw_string.substr(i, j - i);

      j++;
      i = j;

      j = next;

      value = raw_string.substr(i, j - i);
      args.insert(new_arg(key, value));

      i = j = next + 2;
      next = raw_string.find("&&", i);
    }

    while (raw_string[j] != '=' && j < raw_string.size())
      j++;

    if (j == raw_string.size())
      return -1;

    key = raw_string.substr(i, j - i);

    j++;
    i = j;

    j = raw_string.size();

    value = raw_string.substr(i, j - i);
    args.insert(new_arg(key, value));
  }
  return 0;
}

int parse_headers(const string &raw_string, TSPRequestHeadersType &headers) {
  if (raw_string.empty())
    return -1;

  size_t i = 0, j = 0;

  while (isblank(raw_string[j]))
    j++;
  i = j;

  while (!isblank(raw_string[j]) && raw_string[j] != ':')
    j++;

  string key = raw_string.substr(i, j - i);

  while (!isblank(raw_string[j]))
    j++;

  i = j;

  while (j != raw_string.size())
    j++;

  string value = raw_string.substr(i, j - i);

  headers.insert(new_header(key, value));

  return 0;
}
