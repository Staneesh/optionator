#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <cmath>

#include "input.h"
#include "option.hpp"
#include "simulator.h"
#include "varcalc.hpp"

using namespace std;

int main() {

  cin.tie();
  cout.tie();
  ios_base::sync_with_stdio(false);

  cerr << "Hello!" << endl;

  Option<OptionType::CALL, OptionPosition::LONG> call(100, 1);

  LongPut put(100, 1);

  prettyPrint(put);

  vector<OptionBase *> portfolio;
/*
  while (true) {
    cerr << "Please specify another european option:" << endl;

    string line;
    cin >> line;
    Input input = parseIntoParameters(line);

    if (input.type == 'C' && input.position == 'L') {
      portfolio.push_back(new Option<OptionType::CALL, OptionPosition::LONG>(
          input.strike, input.timeToMaturity));
    } else if (input.type == 'P' && input.position == 'L') {
      portfolio.push_back(new Option<OptionType::PUT, OptionPosition::LONG>(
          input.strike, input.timeToMaturity));
    } else if (input.type == 'C' && input.position == 'S') {
      portfolio.push_back(new Option<OptionType::CALL, OptionPosition::SHORT>(
          input.strike, input.timeToMaturity));
    } else if (input.type == 'P' && input.position == 'S') {
      portfolio.push_back(new Option<OptionType::PUT, OptionPosition::SHORT>(
          input.strike, input.timeToMaturity));
    }

    cerr << "payout variance before purchase: " << 0 << endl;
    cerr << "payout variance after purchase: " << 0 << endl;
  }

  for (auto option : portfolio) {
    delete option;
  }
*/
  // test

  double iterations = 10000/*00*/;
  double initialPrice = 5000;
  double years = 1.0;
  double numDays = 365.0;
  double timeStep = 1.0 / 100.0;
  double mu = 0.05;
  double sigma = 0.2;

  StockSimulator simulator(iterations, initialPrice, years, numDays, timeStep, mu, sigma);

  simulator.runSimulation();

  double average = 0.0;

  for (int i=0; i<iterations; i++){
      average += simulator.getPrices()[i][numDays-1];
  }
  average /= iterations;

  double var = 0;
  for (int i = 0; i < iterations; i++ ) {
      var += (simulator.getPrices()[i][numDays-1] - average)*(simulator.getPrices()[i][numDays-1] - average);
  }
  var /= iterations;
  std::cout << "E[x_t] from sim: " << average << std::endl;

  std::cout << "var from simulation " << var << ", from formula " << initialPrice*initialPrice*exp(2.0*(mu/(numDays*years))*years*numDays)*(exp(pow(sigma/(sqrt(numDays*years)),2)*years*numDays)-1.0) << std::endl;

//end of test

  return 0;
}
