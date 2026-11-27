#include "lunar/festivals.hpp"

namespace lunar {

const Festival* find_festival(std::string_view name_or_alias) {
    return nullptr;
}

std::vector<SolarDate> resolve(const Festival& f, int solar_year, double tz)
{
    return std::vector<SolarDate>{};
}

std::vector<Occurence> all_festivals(int solar_year, double tz) {
    return std::vector<Occurence>{};
}

} // namespace lunar
