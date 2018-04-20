#ifndef TSP_LOAD_MODULE_H
#define TSP_LOAD_MODULE_H

#include "../core/response.h"
#include "../include/type.h"

#include <vector>

class TSPBasicResponse;

#define ResponseVectorType std::vector<TSPBasicResponse *>

class TSPModuleManager {
public:
  static ResponseVectorType get() { return response; }

private:
  static ResponseVectorType response;
};

ResponseVectorType install_modules();

#endif
