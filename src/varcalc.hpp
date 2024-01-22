#pragma once
#include "option.hpp"
#include <vector>

struct PayoutProperties {
  double ev;
  double var;
  double stdev;
};

class VarCalc {
public:
  VarCalc() = delete;
  ~VarCalc(){}; // Don't free any memory - data is borrowed, outlives VarCalc!

  VarCalc(const std::vector<std::vector<double>> &p,
          const std::vector<OptionBase *> pf)
      : paths(p), portfolio(pf){};

  PayoutProperties getPayoff();

private:
  std::vector<OptionBase *> portfolio;
  const std::vector<std::vector<double>> &paths;
};
