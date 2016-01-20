# Author(s):
#  - Corentin LE MOLGAT <clemolgat@aldebaran-robotics.com>
# Copyright (C) 2015 Aldebaran Robotics
cmake_minimum_required(VERSION 2.8)
project(Snippet)

##########################
##  INSTALL PARAMETERS  ##
##########################
# Offer the user the choice of overriding the installation directories
set(CMAKE_INSTALL_PREFIX install)
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

##############################
##  COMPILATION PARAMETERS  ##
##############################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)

add_subdirectory(shared_from_this)
add_subdirectory(virtual_init_ctor)
add_subdirectory(custom_iterator)
add_subdirectory(xmacro)
