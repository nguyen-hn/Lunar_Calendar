# Vietnamese Lunar Calendar (C++)

A C++ implementation of **Hồ Ngọc Đức's Vietnamese lunisolar calendar algorithm** (`xemamlich.uhm.vn`), designed for accurate Gregorian-to-Lunar and Lunar-to-Gregorian date conversions, leap month resolution, solar term (*tiết khí*) calculation, and *Can-Chi* (Sexagenary cycle) indexing.

## Overview

Unlike lookup-table-only calendars, this library computes astronomical lunisolar milestones from Julian Day Numbers and ecliptic/lunar phase formulas tailored to Vietnam's standard time zone (`UTC+7`, by default).

### Key Features

- **Bidirectional Conversion:** Convert Gregorian dates (`YYYY-MM-DD`) to Vietnamese Lunar dates (`day`, `month`, `year`, `is_leap_month`) and vice versa.
- **Astronomical Rules:** Handles new moon (*ngày Sóc*) day boundaries, major/minor solar terms (*Trung khí* / *Tiết khí*), and 13-month leap years (placing the leap month after the first month without a *Trung khí* following Winter Solstice).
- **Can-Chi (Sexagenary Cycle):** Computes *Can* (Heavenly Stems) and *Chi* (Earthly Branches) for years, lunar months, and days.
- **Configurable Time Zone:** Defaults to `UTC+7` (`7.0`) while allowing custom time zone offsets for astronomical calculations.
- **Zero Heavy Dependencies:** Written in standard C++ (C++17 or later) suitable for desktop apps, CLI tools, servers, and resource-constrained targets.

## Project Structure

```text
├── apps
│   ├── CMakeLists.txt
│   └── main.cpp
├── build.sh
├── CMakeLists.txt
├── include
│   └── lunar
│       ├── astro.hpp
│       ├── calendar.hpp
│       ├── festivals.hpp
│       ├── solar_terms.hpp
│       └── types.hpp
├── LICENSE
├── README.md
├── src
│   ├── astro.cpp
│   ├── calendar.cpp
│   ├── CMakeLists.txt
│   ├── festivals.cpp
│   └── solar_terms.cpp
└── tests

```

## Building with CMake
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Algorithm & References

    Hồ Ngọc Đức — Thuật toán tính âm lịch (Vietnamese Lunar Calendar Algorithm): https://www.xemamlich.uhm.vn/calrules.html

    Hồ Ngọc Đức — Vietnamese Lunar Calendar Portal: https://www.xemamlich.uhm.vn/

Notes on Historical vs. Astronomical Calendars

Astronomical formulas project modern Vietnamese calendar rules across dates. Historical Vietnamese eras (such as pre-modern dynasties or regional calendar variations prior to nationwide unification) may differ from pure astronomical projections.

## License

Licensed under the **Apache License, Version 2.0**. See the [LICENSE](LICENSE) file for the full license text.
