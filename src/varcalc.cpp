#include "varcalc.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

PayoutProperties VarCalc::getPayoff() {
  std::vector<double> payoffs;
  payoffs.reserve(this->paths.size()); // Every path has a payoff.

  for (auto &path : this->paths) {         // Iterate through all asset paths
    double pathPayoff = 0;                 // Calculate the payoff on every path
    for (auto &option : this->portfolio) { // For every option in our porfolio
      double T = option->getTimeToMaturity();
      double spot = path[static_cast<int>(T)];
      pathPayoff += option->payoff(spot);  // Accumulate individual path payoffs
    }
    payoffs.push_back(pathPayoff);
  }

  double sum = std::accumulate(payoffs.begin(), payoffs.end(), 0.0);
  double mean = sum / payoffs.size();

  double accum = 0.0;
  std::for_each(payoffs.begin(), payoffs.end(),
                [&](const double d) { accum += (d - mean) * (d - mean); });

  double var = accum / (payoffs.size() - 1);
  double stdev = sqrt(var);

  return {.ev = mean, .var = var, .stdev = stdev};
}
