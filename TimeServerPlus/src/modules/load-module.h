#ifndef TSP_LOAD_MODULE_H
#define TSP_LOAD_MODULE_H

#define TSP_MODULE_MAJOR 0
#define TSP_MODULE_MINOR 1
#define TSP_MODULE_PATCH 0
#define TSP_MODULE_SONAME 0.1

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
