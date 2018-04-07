#ifndef TSP_MODULE_TIME_H
#define TSP_MODULE_TIME_H

#include "../response.h"
#include "../type.h"
#include "../utility.h"
#include "load-module.h"

#include <unistd.h>
#include <vector>

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
