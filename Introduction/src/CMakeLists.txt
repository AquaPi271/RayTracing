cmake_minimum_required(VERSION 3.10)

# This gives a name and version to the project.  It also creates
# two variables called baseline_png_VERSION_MAJOR and 
# baseline_png_VERSION_MINOR.
project(baseline_png VERSION 1.0)

configure_file(baseline_png_config.h.in baseline_png_config.h)

# Reserved variable that says the C++ code works only on C++20 or later.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(baseline_png main.cpp)

target_include_directories(baseline_png PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
