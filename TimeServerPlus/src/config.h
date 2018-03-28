#ifndef TSP_CONFIG
#define TSP_CONFIG

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using std::string;

int tsp_config_parse(const std::string path);
int tsp_config_get(const std::string key, std::string &value);

class TSPConfig {
public:
  TSPConfig(const TSPConfig &) = delete;
  TSPConfig &operator=(const TSPConfig &) = delete;

  static TSPConfig *instance() {
    if (config == nullptr) {
      pthread_mutex_lock(&lock);
      if (config == nullptr)
        config = new TSPConfig();
      pthread_mutex_unlock(&lock);
    }
    return config;
  }
  int parse(const string path);
  int get(const string key, string &value);
  int exist(const string key, const string value);

private:
  TSPConfig() {
    data.clear();
    data.insert(std::make_pair("LOG", "TimeServerPlus.log"));
  };
  ~TSPConfig() = default;

  static pthread_mutex_t lock;
  static TSPConfig *config;
  std::map<string, string> data;
};
#endif
