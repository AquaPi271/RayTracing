cmake_minimum_required(VERSION 3.10)

# This gives a name and version to the project.  It also creates
# two variables called baseline_png_VERSION_MAJOR and 
# baseline_png_VERSION_MINOR.

set (program_name "raytrace")

project(${program_name} VERSION 1.0)

configure_file(src/${program_name}_config.h.in ${program_name}_config.h)

# Reserved variable that says the C++ code works only on C++20 or later.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${program_name} src/main.cpp)

target_include_directories(${program_name} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
