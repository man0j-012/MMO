cmake_minimum_required(VERSION 3.16)
project(DistributedMulticastMMO LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_GUI "Build Qt monitor GUI" ON)

add_subdirectory(src)
if (BUILD_GUI)
  add_subdirectory(gui)
endif()
