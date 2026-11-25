#ifndef SOLAR_TERMS_HPP_
#define SOLAR_TERMS_HPP_

#include "types.hpp"

namespace lunar {

struct SolarTerm {
    int index;
    std::string_view name;
    SolarDate date;
};

std::array<SolarTerm, 24> solar_term(int year, double tz);

}

#endif // SOLAR_TERMS_HPP_
