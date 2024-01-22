#include "varcalc.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
//template <OptionType type, OptionPosition position>
PayoutProperties VarCalc::getPayoff() {
  std::vector<double> payoffs;
  payoffs.reserve(this->paths.size()); // Every path has a payoff.

  double pathPayoff = 0;                 // Calculate the payoff on every path
  for (auto &option_ : this->portfolio) { // For every option in our porfolio
    //TODO - couldn't make it work
    //Option<type, position>* option = dynamic_cast<Option<type, position>*>(option_);
    double T = option->getTimetoMaturity(); //added in option.hpp
    for (int i = 0; i<paths.size(); i++){
      pathPayoff += option->payoff(paths[i][T]); // Accumulate individual path payoffs
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
