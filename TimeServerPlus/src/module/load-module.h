#ifndef TSP_LOAD_MODULE_H
#define TSP_LOAD_MODULE_H

#include "../response.h"
#include "../type.h"

#include <regex>
#include <string>
#include <vector>

static std::vector<tsp_module_t> tsp_module_vector;

void tsp_module_init();
std::vector<tsp_module_t> tsp_module_get();
void tsp_module_sort_by_priority();

class TSPBasicResponse;

#define ResponseVectorType std::vector<TSPBasicResponse>

class TSPModuleManager {
public:
  TSPModuleManager() = default;
  ~TSPModuleManager() = default;
  static ResponseVectorType get() { return response; }

private:
  static ResponseVectorType response;
};

ResponseVectorType install_modules();

#endif
