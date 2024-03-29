#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>

#include "input.h"
#include "option.hpp"
#include "simulator.h"
#include "varcalc.hpp"

using namespace std;

int main() {


  cin.tie();
  cout.tie();
  ios_base::sync_with_stdio(false);

  //.CSV LOADING
  //path to the .csv
  std::string sample = "/home/nadim/sample/sample_large.csv";

  //opening the .csv
  std::ifstream file(sample);

  //check if the .csv opened successfully
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << sample << std::endl;
    return 1;
  }

  //vector storing sample data
  std::vector<std::string> sample_data;

  std::string line;
  while (std::getline(file, line)) {
    sample_data.push_back(line);
  }

  file.close();
  //END OF .CSV LOADING



  cerr << "Hello! Running simulations first..." << endl;

  double iterations = 2000000;
  double initialPrice = 5000;
  double numDays = 60;
  double timeStep = 1.0 / 100.0;
  double mu = 0.05;
  double sigma = 0.2;
  double variance_cutoff;

  StockSimulator simulator(iterations, initialPrice, numDays, timeStep,
                           mu, sigma);

  auto startTime = std::chrono::high_resolution_clock::now();

  simulator.runSimulation();

  auto endTime = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      endTime - startTime);
  cerr << "Function duration: " << duration.count() << " milliseconds"
            << std::endl;

  vector<OptionBase *> portfolio;

  cerr << "Specify variance cutoff: ";
  std::cin >> variance_cutoff;

  int Trades=0;
  cerr << "Trade: " << 0 << std::endl;
  cerr << "Expected Value: " << 0 << std::endl;
  cerr << "Variance: " << 0 << std::endl;
  cerr << "Standard Deviation: " << 0 << std::endl;

  for(const auto& element : sample_data){

    Input input = parseIntoParameters(element);
    for(int i=0; i<input.volume; i++){
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
    }
    // Create an instance of VarCalc
    VarCalc varCalculator(simulator.getPrices(), portfolio);

    // Calculate and retrieve payout properties
    PayoutProperties payoutProps = varCalculator.getPayoff();

    // Access the properties
    double expectedValue = payoutProps.ev;
    double variance = payoutProps.var;
    double standardDeviation = payoutProps.stdev;

    Trades++;

    cerr << std::endl << "Trade: " << Trades << std::endl;
    cerr << "Expected Value: " << expectedValue << std::endl;
    cerr << "Variance: " << variance << std::endl;
    cerr << "Standard Deviation: " << standardDeviation << std::endl;

    if(variance_cutoff < variance){
      cerr << "Variance cutoff exceeded, terminating program." << std::endl;
      break;}
  }


/*
  while (true) {
    cerr << "Please specify another european option (type, position, strike, "
            "ttm, volume)"
         << endl;

    string line;
    cin >> line;
    Input input = parseIntoParameters(line);
    for(int i=0; i<input.volume; i++){
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
    }
    // Create an instance of VarCalc
    VarCalc varCalculator(simulator.getPrices(), portfolio);

    // Calculate and retrieve payout properties
    PayoutProperties payoutProps = varCalculator.getPayoff();

    // Access the properties
    double expectedValue = payoutProps.ev;
    double variance = payoutProps.var;
    double standardDeviation = payoutProps.stdev;

    Trades++;

    std::cout << "Trades: " << Trades << std::endl;
    std::cout << "Expected Value: " << expectedValue << std::endl;
    std::cout << "Variance: " << variance << std::endl;
    std::cout << "Standard Deviation: " << standardDeviation << std::endl;

    if(variance_cutoff < variance){
      std::cout << "Variance cutoff exceeded, terminating program." << std::endl;
      break;}
  }

  portfolio.clear();

  if (0) {
    // test

    double iterations = 1000000;
    double initialPrice = 5000;
    double years = 1.0;
    double numDays = 365.0;
    double timeStep = 1.0 / 100.0;
    double mu = 0.05;
    double sigma = 0.2;

    StockSimulator simulator(iterations, initialPrice, years, numDays, timeStep,
                             mu, sigma);

    auto startTime = std::chrono::high_resolution_clock::now();

    simulator.runSimulation();

    auto endTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime);
    std::cout << "Function duration: " << duration.count() << " milliseconds"
              << std::endl;

    //before multi-threading around 30000 miliseconds for double iterations =
    //2000000; double initialPrice = 5000; double years = 1.0; double numDays
    //= 2.0; double timeStep = 1.0 / 400.0; double mu = 0.05; double sigma = 0.2;

    //after multi-threading around 3000 miliseconds


    double average = 0.0;

    for (int i = 0; i < iterations; i++) {
      average += simulator.getPrices()[i][numDays - 1];
    }
    average /= iterations;

    double var = 0;
    for (int i = 0; i < iterations; i++) {
      var += (simulator.getPrices()[i][numDays - 1] - average) *
             (simulator.getPrices()[i][numDays - 1] - average);
    }
    var /= iterations;
    std::cout << "E[x_t] from sim: " << average << ", from formula "
              << initialPrice * exp(mu / (365 * years) * years * numDays)
              << std::endl;

    std::cout << "var from simulation " << var << ", from formula "
              << initialPrice * initialPrice *
                     exp(2.0 * (mu / (365 * years)) * years * numDays) *
                     (exp(pow(sigma / (sqrt(365 * years)), 2) * years *
                          numDays) -
                      1.0)
              << std::endl;

    // end of test

    LongCall callOption1(5000, 60);
    LongCall callOption2(5000, 60);
    LongCall callOption3(5000, 60);
    LongCall callOption4(5000, 60);
    LongCall callOption5(5000, 60);

    // Create a portfolio of options
    std::vector<OptionBase *> optionPortfolio = {
        &callOption1, &callOption2, &callOption3, &callOption4, &callOption5};

    // Create an instance of VarCalc
    VarCalc varCalculator(simulator.getPrices(), optionPortfolio);

    // Calculate and retrieve payout properties
    PayoutProperties payoutProps = varCalculator.getPayoff();

    // Access the properties
    double expectedValue = payoutProps.ev;
    double variance = payoutProps.var;
    double standardDeviation = payoutProps.stdev;

    std::cout << "Expected Value: " << expectedValue << std::endl;
    std::cout << "Variance: " << variance << std::endl;
    std::cout << "Standard Deviation: " << standardDeviation << std::endl;
  };
*/
  return 0;
}
