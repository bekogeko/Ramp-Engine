cmake_minimum_required(VERSION 3.2)
project(stb_image)

cmake_policy(SET CMP0115 OLD)
add_library(stb_image)
target_sources(stb_image PRIVATE "src/stb_image")
target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")



