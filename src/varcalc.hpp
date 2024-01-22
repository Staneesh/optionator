#pragma once
#include "option.hpp"
#include <vector>

struct PayoutProperties {
  double ev;
  double var;
};

class VarCalc {
public:
  VarCalc(const std::vector<std::vector<double>> &p,
          const std::vector<OptionBase *> pf)
      : paths(p), portfolio(pf){};

  double getPayout();

private:
  std::vector<OptionBase *> portfolio;
  std::vector<std::vector<double>> paths;
};
