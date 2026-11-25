add_library(lunar STATIC astro.cpp calendar.cpp solar_terms.cpp festivals.cpp)
target_include_directories(lunar PUBLIC ${PROJECT_SOURCE_DIR}/include/lunar)
target_compile_options(lunar PRIVATE -Wall -Wextra -Werror=return-type)
