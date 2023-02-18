cmake_minimum_required(VERSION 3.10)

# This gives a name and version to the project.  It also creates
# two variables called baseline_png_VERSION_MAJOR and 
# baseline_png_VERSION_MINOR.
project(baseline_vec3 VERSION 1.0)

configure_file(src/baseline_vec3_config.h.in baseline_vec3_config.h)

# Reserved variable that says the C++ code works only on C++20 or later.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(baseline_vec3 src/main.cpp)

target_include_directories(baseline_vec3 PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
