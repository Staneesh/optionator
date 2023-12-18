#include <iostream>

#include "option.hpp"

using namespace std;

int main() {
  cin.tie();
  cout.tie();
  ios_base::sync_with_stdio(false);

  cerr << "Hello!" << endl;

  Option<OptionType::CALL> call(100, 1);

  PutOption put(100, 1);

  return 0;
}
