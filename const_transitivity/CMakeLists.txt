set(_SRCS main.cpp)
add_executable(ConstTransitivity ${_SRCS})

if(BUILD_TESTING)
  add_test(NAME cxx_ConstTransitivity COMMAND ConstTransitivity)
endif()
