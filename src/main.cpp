#include <iostream>
#include <sstream>
#include <vector>
#include <random>

#include "input.h"
#include "option.hpp"
#include "simulator.h"

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

  double iterations = 10/*0000*/;
  double initialPrice = 5000;
  double years = 1.0;
  double numDays = /*365.0*/ 3;
  double timeStep = 1.0 / (40.0 * numDays);
  double mu = 0.05;
  double sigma = 0.2;

  StockSimulator simulator(iterations, initialPrice, years, numDays, timeStep, mu, sigma);

  simulator.runSimulation();

  cout << simulator.getPrices()[1][1];
  //end of test

  return 0;
}
