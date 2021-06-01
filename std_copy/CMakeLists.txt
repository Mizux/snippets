set(_SRCS main.cpp)
add_executable(STLCopy ${_SRCS})

if(BUILD_TESTING)
  add_test(NAME cxx_STLCopy COMMAND STLCopy)
endif()
