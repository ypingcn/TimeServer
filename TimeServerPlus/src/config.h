#ifndef TSP_CONFIG
#define TSP_CONFIG

#include <map>
#include <string>

#include <fstream>
#include <sstream>
#include <iostream>

int tsp_config_parse(const std::string path);
int tsp_config_get(const std::string key,std::string &value);

#endif
