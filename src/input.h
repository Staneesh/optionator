#pragma once
#include <string>

struct Input {
  double strike, timeToMaturity, volume;
  char type, position;
};

Input parseIntoParameters(const std::string &line);
