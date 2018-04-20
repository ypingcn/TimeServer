#include "load-module.h"

#include "module_basic.h"
#include "module_time.h"

ResponseVectorType TSPModuleManager::response = install_modules();

ResponseVectorType install_modules() {
  ResponseVectorType res;
  install_basic_modules(res);
  install_time_modules(res);
  sort(res.begin(), res.end(),
       [](TSPBasicResponse *a, TSPBasicResponse *b) -> bool {
         return a->get_priority() > b->get_priority();
       });
  return res;
}
