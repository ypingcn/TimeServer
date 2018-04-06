#ifndef TSP_MODULE_BASIC_H
#define TSP_MODULE_BASIC_H

#include "../config.h"
#include "../response.h"
#include "../type.h"

#include <regex>
#include <string>
#include <sys/sendfile.h>
#include <unistd.h>

using std::regex;
using std::string;

void tsp_module_basic_init(std::vector<tsp_module_t> &v);

void *tsp_module_basic_get(void *sockfd, void *request);
void *tsp_module_basic_head(void *sockfd, void *request);
void *tsp_module_basic_options(void *sockfd, void *request);

void *tsp_module_basic_post(void *sockfd, void *request);
void *tsp_module_basic_put(void *sockfd, void *request);
void *tsp_module_basic_delete(void *sockfd, void *request);

class TSPBasicGetResponse : public TSPBasicResponse {
public:
  TSPBasicGetResponse(int _priority, TSPResponsePatternType _pattern)
      : TSPBasicResponse(_priority, _pattern) {}
  ~TSPBasicGetResponse(){};

  int handle(const TSPRequest &req) final;
};

class TSPBasicHeadResponse : public TSPBasicResponse {
public:
  TSPBasicHeadResponse(int _priority, TSPResponsePatternType _pattern)
      : TSPBasicResponse(_priority, _pattern) {}
  ~TSPBasicHeadResponse(){};

  int handle(const TSPRequest &req) final;
};

class TSPBasicOptionsResponse : public TSPBasicResponse {
public:
  TSPBasicOptionsResponse(int _priority, TSPResponsePatternType _pattern)
      : TSPBasicResponse(_priority, _pattern) {}
  ~TSPBasicOptionsResponse(){};

  int handle(const TSPRequest &req) final;
};

class TSPBasicPostResponse : public TSPBasicResponse {
public:
  TSPBasicPostResponse(int _priority, TSPResponsePatternType _pattern)
      : TSPBasicResponse(_priority, _pattern) {}
  ~TSPBasicPostResponse(){};

  int handle(const TSPRequest &req) final;
};

class TSPBasicPutResponse : public TSPBasicResponse {
public:
  TSPBasicPutResponse(int _priority, TSPResponsePatternType _pattern)
      : TSPBasicResponse(_priority, _pattern) {}
  ~TSPBasicPutResponse(){};

  int handle(const TSPRequest &req) final;
};

class TSPBasicDeleteResponse : public TSPBasicResponse {
public:
  TSPBasicDeleteResponse(int _priority, TSPResponsePatternType _pattern)
      : TSPBasicResponse(_priority, _pattern) {}
  ~TSPBasicDeleteResponse(){};

  int handle(const TSPRequest &req) final;
};

void install_basic_modules(ResponseVectorType &t);

#endif
