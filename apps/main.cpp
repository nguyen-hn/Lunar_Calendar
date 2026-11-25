#include <iostream>
#include "calendar.hpp"

int main(int argc, char* argv[]) {
    lunar::LunarDate lunar_date = lunar::to_lunar(lunar::SolarDate{25, 11, 2026}, 7.0);
    std::cout << "Lunar: " << lunar_date.day << "/" << lunar_date.month << "/" << lunar_date.year << "\n";
    return 0;
}
