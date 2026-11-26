include (FetchContent)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)

FetchContent_MakeAvailable(argparse)

add_executable(lunar-cli main.cpp)
target_include_directories(lunar-cli PRIVATE "${PROJECT_SOURCE_DIR}/include")
# target_link_directories(lunar-cli PRIVATE "${PROJECT_SOURCE_DIR}/build/src")
target_link_libraries(lunar-cli PRIVATE lunar argparse)
