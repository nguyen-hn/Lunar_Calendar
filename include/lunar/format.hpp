#ifndef LUNAR_FORMAT_HPP_
#define LUNAR_FORMAT_HPP_

#include <iosfwd>
#include <string>
#include "types.hpp"
#include "solar_terms.hpp"

namespace lunar {
std::ostream& operator<<(std::ostream& out, const SolarDate& solar_date);
std::ostream& operator<<(std::ostream& out, const LunarDate& lunar_date);
std::ostream& operator<<(std::ostream& out, const SolarTerm& solar_term);

std::string to_string(const SolarDate& solar_date);
std::string to_string(const LunarDate& lunar_date);
std::string to_string(const SolarTerm& solar_term);


std::optional<SolarDate> parse_solar(std::string_view str);
std::optional<LunarDate> parse_lunar(std::string_view str);
}

#endif // LUNAR_FORMAT_HPP_
