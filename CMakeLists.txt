## Author(s):
##  - Corentin LE MOLGAT <clemolgat@aldebaran-robotics.com>
## Copyright (C) 2015 Aldebaran Robotics
cmake_minimum_required(VERSION 2.8)
project(Snippet)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(_SRCS main.cpp)
add_executable(${PROJECT_NAME} ${_SRCS})
