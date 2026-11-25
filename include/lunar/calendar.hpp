#ifndef CALENDAR_HPP_
#define CALENDAR_HPP_

#include "types.hpp"

namespace lunar {
bool is_valid(SolarDate d);
LunarDate to_lunar(SolarDate d, double tz);
std::optional<SolarDate> to_solar(LunarDate d, double tz);
}

#endif // CALENDAR_HPP_
