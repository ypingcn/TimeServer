#include "load-module.h"

#include "module_basic.h"
#include "module_time.h"

void tsp_module_init()
{
  if(!tsp_module_vector.empty())
    tsp_module_vector.clear();

  tsp_module_time_init(tsp_module_vector);
  tsp_module_basic_init(tsp_module_vector);

  tsp_module_sort_by_priority();
}

std::vector<tsp_module_t> tsp_module_get()
{
  return tsp_module_vector;
}

void tsp_module_sort_by_priority()
{
  sort(tsp_module_vector.begin(),tsp_module_vector.end(),
    [](tsp_module_t a,tsp_module_t b){ return a.priority > b.priority;});
}
