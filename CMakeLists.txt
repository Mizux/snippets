cmake_minimum_required(VERSION 3.2)
project(Snippet)

##########################
##  INSTALL PARAMETERS  ##
##########################
# Offer the user the choice of overriding the installation directories
set(CMAKE_INSTALL_PREFIX install)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/")

##############################
##  COMPILATION PARAMETERS  ##
##############################
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_subdirectory(const_transitivity)
add_subdirectory(shared_from_this)
add_subdirectory(virtual_init_ctor)
add_subdirectory(custom_iterator)
add_subdirectory(xmacro)
add_subdirectory(find_tuple)
