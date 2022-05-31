cmake_minimum_required(VERSION 3.13)
project(myproject)

set(CMAKE_CXX_STANDARD 14)

add_executable(myproject stb_image.h utils.cpp spirte.h map.cpp framebuffer.cpp textures.cpp player.h   main.cpp )