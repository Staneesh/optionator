#include "option.hpp"

// bodies of generic methods need to be defined in the .hpp file!

template <>
double Option<OptionType::CALL, OptionPosition::LONG>::payoffImpl(double spot) const {
  return fmax(spot - this->m_strikePrice, 0);
}

template <>
double Option<OptionType::CALL, OptionPosition::SHORT>::payoffImpl(double spot) const {
  return fmin(this->m_strikePrice - spot, 0);
}

template <>
double Option<OptionType::PUT, OptionPosition::LONG>::payoffImpl(double spot) const {
  return fmax(this->m_strikePrice - spot, 0);
}

template <>
double Option<OptionType::PUT, OptionPosition::SHORT>::payoffImpl(double spot) const {
  return fmin(spot - this->m_strikePrice, 0);
}
