#ifndef TSP_MODULE_TIME_H
#define TSP_MODULE_TIME_H

#include "../contrib/utility.h"
#include "../core/response.h"
#include "../include/type.h"
#include "load-module.h"

class TSPTimeLocaltimeResponse : public TSPBasicResponse {
public:
  TSPTimeLocaltimeResponse(int _priority, TSPResponsePatternType _type)
      : TSPBasicResponse(_priority, _type) {}
  ~TSPTimeLocaltimeResponse() {}

  int handle(const TSPRequest &req) final;
};

class TSPTimeFileCtimeResponse : public TSPBasicResponse {
public:
  TSPTimeFileCtimeResponse(int _priority, TSPResponsePatternType _type)
      : TSPBasicResponse(_priority, _type) {}
  ~TSPTimeFileCtimeResponse() {}

  int handle(const TSPRequest &req) final;
};

class TSPTimeFileAtimeResponse : public TSPBasicResponse {
public:
  TSPTimeFileAtimeResponse(int _priority, TSPResponsePatternType _type)
      : TSPBasicResponse(_priority, _type) {}
  ~TSPTimeFileAtimeResponse() {}

  int handle(const TSPRequest &req) final;
};

class TSPTimeFileMtimeResponse : public TSPBasicResponse {
public:
  TSPTimeFileMtimeResponse(int _priority, TSPResponsePatternType _type)
      : TSPBasicResponse(_priority, _type) {}
  ~TSPTimeFileMtimeResponse() {}

  int handle(const TSPRequest &req) final;
};

void install_time_modules(ResponseVectorType &t);

#endif
