#include "input.h"

#include <sstream>
#include <vector>

Input parseIntoParameters(const std::string &line) {
  std::stringstream ss(line);

  std::string token;
  std::vector<std::string> elems;

  while (std::getline(ss, token, ',')) {
    elems.push_back(token);
  }

  return Input{
      .strike = stod(elems[2]),
      .timeToMaturity = stod(elems[3]),
      .volume = stod(elems[4]),
      .type = elems[0][0],
      .position = elems[1][0],
  };
}
