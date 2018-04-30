#include "config.h"
#include "logger.h"

#include <fstream>
#include <sstream>

TSPConfig *TSPConfig::config = nullptr;
pthread_mutex_t TSPConfig::lock = PTHREAD_MUTEX_INITIALIZER;

int TSPConfig::parse(const string path) {
  std::fstream config_file(path, std::fstream::in);
  if (!config_file) {
    cout << "config error" << endl;

    config_file.close();
    return -1;
  }

  string line, key, value;
  config->data.clear();
  while (getline(config_file, line)) {
    std::stringstream stream(line);
    stream >> key >> value;
    if (key != "" && value != "" &&
        config->data.find("key") == config->data.end())
      config->data.insert(make_pair(key, value));
  }
  config_file.close();
  return 0;
}

int TSPConfig::get(const string key, string &value) {
  map<string, string>::const_iterator it = config->data.find(key);
  if (it == data.end()) {
    cout << "no exist key:" << key << endl;
    return -1;
  }
  value = it->second;
  return 0;
}

int TSPConfig::exist(const string key, const string value) {
  map<string, string>::const_iterator it = config->data.find(key);
  if (it == data.end()) {
    cout << "no exist key:" << key << endl;
    return -1;
  }
  return it->second == value;
}
