#include "request.h"

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

  if ((nxt = raw_request.find(rn, pre)) != string::npos) {
    string line(raw_request.substr(pre, nxt - pre));
    cout << line << endl;
    std::stringstream s(line);
    s >> method;
    s >> url;
    s >> version;

    if (version.find("HTTP", 0) == string::npos)
      return -1;

    string url_line(url);
    if (url_line.find("..", 0) != string::npos)
      url_line = "/";

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
    while (next != string::npos) {
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

void tsp_request_handle(const TSPRequest &req) { tsp_response(req); }
