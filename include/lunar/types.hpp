#ifndef TYPES_HPP_
#define TYPES_HPP_ 

#include <vector>
#include <array>
#include <string_view>
#include <optional>

namespace lunar {
struct SolarDate {
    int day, month, year;
};

struct LunarDate {
    int day, month, year;
    bool leap;
};
}

#endif // TYPES_HPP_
#define TYPES_HPP_ 
