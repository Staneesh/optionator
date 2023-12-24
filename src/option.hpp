#pragma once

// This enum is specifying if the option is a call or a put.
// Currently it assumes that the underlying option is European.
// This enum is exported whenever you include this header, making
// it available to use this to create objects of type Option<Option Type>.
enum OptionType {
  CALL,
  PUT,
};

// The below block defines a class template - you can imagine it defines
// a whole family of classes called 'Option', such that the OptionType can
// assume either one of two values: CALL or PUT. You specify the Option class
// that you want to instantiate (create object of) by these template arguments:
// Option<OptionType::CALL> myOptionObject( /* strike price = */     10,
//                                          /* time to maturity = */ 1  );
// or:
// Option<OptionType::PUT> myPut(10, 1);
// or:
// PutOption myPut(10, 1); <- alias: using PutOption = Option<OptionType::PUT>;
//
// Notice how the template argumet OptionType holds some important information
// about the option. As opposed to storing this option type in the member field
// of the class, encodes it directly in the type of the object. We really have
// separate types for CALL and PUT options, they are just implemented using
// a single definition of a template.
//
// The advantage of that approach is type safety - imagine there is a function
// somewhere called "convert_to_put" that would accept an option which is not
// generic over the OptionType (and just stores this in a field). If we wanted
// this funciton to be undefined for put options, we would have to raise
// some sort of a runtime exception if object.type == "PUT". This will really
// only be caught during the execution of the program (the runtime), which could
// pessimistically fail very late, if "convert_to_put" is called after heavy
// computations, wasting our time.
//
// We could instead implement "convert_to_put" only for Option<OptionType::CALL>
// now that we have this distinction in the type itself. This has a huge
// advantage of not even compiling when bad type is provided as an argument,
// because the compiler would be checking all the types passed to functions. So:
//
// Option<OptionType::PUT> put(10, 1);
// auto result = convert_to_put( put ); <- This will not COMPILE
//
// We could still implement functions for all Options like that:
// template<OptionType type>
// void prettyPrint(Option<type> anyOption) {
//   std::cerr << anyOption.price(); // <- .price() implemented for all options
// }
// prettyPrint(put); // <- notice how normal the call looks - no template args
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

// Generic methods must be in .hpp!
template <OptionType type>
Option<type>::Option(double strikePrice, double timeToMaturity)
    : m_strikePrice(strikePrice), m_timeToMaturity(timeToMaturity),
      m_flags(initializeFlags()){};

// Useful aliases for common Option configurations:
using CallOption = Option<OptionType::CALL>;
using PutOption = Option<OptionType::PUT>;

// Just to ilustrate the example from the comment of class "Option":
template <OptionType type> void prettyPrint(Option<type> anyOption) {}
