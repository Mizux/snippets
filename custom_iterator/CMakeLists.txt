cmake_minimum_required(VERSION 3.2)

set(_NAME CustomIterator)

file(GLOB_RECURSE _HDRS include *.hpp *.hxx)
file(GLOB_RECURSE _SRCS src/*.[hc]pp)

add_executable(${_NAME} ${_HDRS} ${_SRCS})
target_include_directories(${_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if(BUILD_TESTING)
  add_test(NAME cxx_${_NAME} COMMAND ${_NAME})
endif()
