cmake_minimum_required(VERSION 3.1)
project(clay)

add_library(clay INTERFACE)


target_include_directories(clay INTERFACE clay "${CMAKE_CURRENT_SOURCE_DIR}/")
