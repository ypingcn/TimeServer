#ifndef TSP_LOAD_MODULE_H
#define TSP_LOAD_MODULE_H

#include "../type.h"

#include <regex>
#include <string>
#include <vector>

static std::vector<tsp_module_t> tsp_module_vector;

void tsp_module_init();
std::vector<tsp_module_t> tsp_module_get();
void tsp_module_sort_by_priority();

#endif
