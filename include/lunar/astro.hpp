#ifndef ASTRO_HPP_
#define ASTRO_HPP_

#include "types.hpp"

namespace lunar {

int jdn_from_date(SolarDate d);
SolarDate date_from_jdn(int jdn);
int new_moon_day(int k, double tz);
double sun_longtitude_deg(int jdn, double tz);
int sun_sector(int jdn, double tz);

}

#endif // ASTRO_HPP_
#define ASTRO_HPP_
