#ifndef LUNAR_TYPES_HPP_
#define LUNAR_TYPES_HPP_ 

#include <vector>
#include <array>
#include <string_view>
#include <optional>
#include <cmath>
#include <initializer_list>

namespace lunar {
struct SolarDate {
    int day, month, year;
};

struct LunarDate {
    int day, month, year;
    bool leap;
};
}

#endif // LUNAR_TYPES_HPP_
