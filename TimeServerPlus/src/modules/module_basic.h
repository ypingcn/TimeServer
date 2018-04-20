#ifndef TSP_MODULE_BASIC_H
#define TSP_MODULE_BASIC_H

#include "../contrib/config.h"
#include "../contrib/logger.h"
#include "../core/response.h"
#include "../include/type.h"
#include "load-module.h"

#include <sys/sendfile.h>
#include <unistd.h>

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
