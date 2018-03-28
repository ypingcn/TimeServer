#include "config.h"
#include "logger.h"

static std::map<std::string, std::string> tsp_config_map;

int tsp_config_parse(const std::string path) {
  std::fstream config_file(path, std::fstream::in);
  if (!config_file) {
    std::cout << "config error" << std::endl;
    config_file.close();
    return -1;
  }

  std::string line;

  while (getline(config_file, line)) {
    std::stringstream stream(line);

    std::string key, value;
    stream >> key >> value;

    tsp_config_map.insert(make_pair(key, value));
  }

  config_file.close();
  return 0;
}

int tsp_config_get(const std::string key, std::string &value) {
  std::map<std::string, std::string>::const_iterator it =
      tsp_config_map.find(key);
  if (it == tsp_config_map.end()) {
    std::cout << "no exist key:" << key << std::endl;
    return -1;
  }
  value = it->second;
  return 0;
}

TSPConfig *TSPConfig::config = nullptr;
pthread_mutex_t TSPConfig::lock = PTHREAD_MUTEX_INITIALIZER;

int TSPConfig::parse(const string path) {
  std::fstream config_file(path, std::fstream::in);
  if (!config_file) {
    std::cout << "config error" << std::endl;

    config_file.close();
    return -1;
  }

  std::string line;
  config->data.clear();
  while (getline(config_file, line)) {
    std::stringstream stream(line);
    std::string key, value;
    stream >> key >> value;
    if (key != "" && value != "" &&
        config->data.find("key") == config->data.end())
      config->data.insert(make_pair(key, value));
  }
  config_file.close();
  return 0;
}

int TSPConfig::get(const string key, string &value) {
  std::map<string, string>::const_iterator it = config->data.find(key);
  if (it == data.end()) {
    std::cout << "no exist key:" << key << std::endl;
    return -1;
  }
  value = it->second;
  return 0;
}

int TSPConfig::exist(const string key, const string value) {
  std::map<string, string>::const_iterator it = config->data.find(key);
  std::cout << key << std::endl;
  if (it == data.end()) {
    std::cout << "no exist key:" << key << std::endl;
    return -1;
  }
  return it->second == value;
}
