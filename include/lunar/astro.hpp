#ifndef ASTRO_HPP_
#define ASTRO_HPP_

#include "types.hpp"

namespace lunar {

int jdn_from_date(SolarDate d);
SolarDate date_from_jdn(int jdn);
int new_moon_day(int k, double tz);
double sun_longitude_deg(double jd_ut);
int sun_sector(int jdn, double tz);
double apparent_sun_longitude_deg(double jd);
double term_instant(double target_deg, double jd_guess);

}

#endif // ASTRO_HPP_
