#pragma once

enum OptionType {
  CALL,
  PUT,
};

template <OptionType type> class Option {
public:
  Option(double strikePrice, double timeToMaturity);

  double price(double spotPrice, double volatility, double riskFreeRate) const;

private:
  static unsigned initializeFlags();
  // Helper method to calculate cumulative distribution function (CDF) of the
  // standard normal distribution
  double N(double x) const;

  // Helper method to calculate d1 in the Black-Scholes formula
  double calculateD1(double spotPrice, double volatility,
                     double riskFreeRate) const;

  // Helper method to calculate d2 in the Black-Scholes formula
  double calculateD2(double spotPrice, double volatility,
                     double riskFreeRate) const;

  // Method to calculate call option price
  double europeanCallPrice(double spotPrice, double volatility,
                           double riskFreeRate) const;

  // Method to calculate put option price via put-call parity
  double europeanPutPrice(double spotPrice, double volatility,
                          double riskFreeRate) const;

  // Member variables - `m` stands for Member - useful as C++ does not enforce
  // `this.value` notation, so the `m_` helps us tell members variables apart.
  const double m_strikePrice;
  const double m_timeToMaturity;

  /*
    `m_flags` is a two-bit member storing boolean properties of the option:

    bit 0: if 0 - uninitialized, if 1 - initialized
    bit 1: if 0 - call, if 1 - put
  */
  const unsigned m_flags : 2;
};

// A static method must be defined in a header:
template <OptionType type> unsigned Option<type>::initializeFlags() {
  unsigned flags = 1;
  flags |= ((type == OptionType::PUT) << 1);
  return (flags);
};

// Generic methods must be in .hpp...?
template <OptionType type>
Option<type>::Option(double strikePrice, double timeToMaturity)
    : m_strikePrice(strikePrice), m_timeToMaturity(timeToMaturity),
      m_flags(initializeFlags()){};

// Useful aliases for common Option configurations:
using CallOption = Option<OptionType::CALL>;
using PutOption = Option<OptionType::PUT>;
