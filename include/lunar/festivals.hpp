#ifndef FESTIVALS_HPP_
#define FESTIVALS_HPP_

#include "types.hpp"

namespace lunar {
enum class Rule {
    Lunar, Solar, Term, Special
};

struct Festival {
    std::string_view name;
    std::initializer_list<std::string_view> aliases;
    Rule rule;
    int a,b;
    std::optional<SolarDate> (*special)(int lunar_year, double tz);
};

struct Occurence {
    const Festival* festival;
    SolarDate date;
};

const Festival* find_festival(std::string_view name_or_alias);
std::vector<SolarDate> resolve(const Festival& f, int solar_year, double tz);
std::vector<Occurence> all_festivals(int solar_year, double tz);

} // namespace FESTIVALS_HPP_

#endif // FESTIVALS_HPP_
