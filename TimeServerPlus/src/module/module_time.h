#ifndef TSP_MODULE_TIME_H
#define TSP_MODULE_TIME_H

#include "../response.h"
#include "../type.h"

#include <string>
#include <unistd.h>
#include <vector>

using std::regex;
using std::string;

void tsp_module_time_init(std::vector<tsp_module_t> &v);

void *tsp_module_time_localtime(void *sockfd, void *request);
void *tsp_module_time_file_ctime(void *sockfd, void *request);
void *tsp_module_time_file_atime(void *sockfd, void *request);
void *tsp_module_time_file_mtime(void *sockfd, void *request);

class TSPTimeLocaltimeResponse : public TSPBasicResponse {
public:
  TSPTimeLocaltimeResponse(int _priority, TSPResponsePatternType _type)
      : TSPBasicResponse(_priority, _type) {}
  ~TSPTimeLocaltimeResponse() {}

  int handle(TSPRequest &req) final;
};

class TSPTimeFileCtimeResponse : public TSPBasicResponse {
public:
  TSPTimeFileCtimeResponse(int _priority, TSPResponsePatternType _type)
      : TSPBasicResponse(_priority, _type) {}
  ~TSPTimeFileCtimeResponse() {}

  int handle(TSPRequest &req) final;
};

class TSPTimeFileAtimeResponse : public TSPBasicResponse {
public:
  TSPTimeFileAtimeResponse(int _priority, TSPResponsePatternType _type)
      : TSPBasicResponse(_priority, _type) {}
  ~TSPTimeFileAtimeResponse() {}

  int handle(TSPRequest &req) final;
};

class TSPTimeFileMtimeResponse : public TSPBasicResponse {
public:
  TSPTimeFileMtimeResponse(int _priority, TSPResponsePatternType _type)
      : TSPBasicResponse(_priority, _type) {}
  ~TSPTimeFileMtimeResponse() {}

  int handle(TSPRequest &req) final;
};

void install_time_modules(ResponseVectorType &t);

#endif
