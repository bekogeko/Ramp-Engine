cmake_minimum_required(VERSION 3.5)
project(clay)

add_library(clay INTERFACE)

# c99 standard
target_compile_features(clay INTERFACE c_std_99)
# c++20 standard
target_compile_features(clay INTERFACE cxx_std_20)

target_include_directories(clay INTERFACE clay "${CMAKE_CURRENT_SOURCE_DIR}/")
