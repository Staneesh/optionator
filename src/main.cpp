#include <iostream>
#include <sstream>
#include <vector>

#include "option.hpp"

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

  while (true) {
    string line;
    cin >> line;
    std::stringstream ss(line);
    std::string token;
    vector<string> elems;
    while (std::getline(ss, token, ',')) {
      elems.push_back(token);
    }

    string type = elems[0], position = elems[1];
    double strike = stod(elems[2]), timeToMaturity = stod(elems[3]),
           volume = stod(elems[4]);

    if (type == "C" && position == "L") {
      portfolio.push_back(new Option<OptionType::CALL, OptionPosition::LONG>(
          strike, timeToMaturity));
    } else if (type == "P" && position == "L") {
      portfolio.push_back(new Option<OptionType::PUT, OptionPosition::LONG>(
          strike, timeToMaturity));
    } else if (type == "C" && position == "S") {
      portfolio.push_back(new Option<OptionType::CALL, OptionPosition::SHORT>(
          strike, timeToMaturity));
    } else if (type == "P" && position == "S") {
      portfolio.push_back(new Option<OptionType::PUT, OptionPosition::SHORT>(
          strike, timeToMaturity));
    }

    cout << "payout variance before purchase: " << 0 << endl;
  }

  for (auto option : portfolio) {
    delete option;
  }

  return 0;
}
