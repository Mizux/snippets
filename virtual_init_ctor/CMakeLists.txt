set(_SRCS main.cpp)
add_executable(VirtualInitCtor ${_SRCS})

if(BUILD_TESTING)
  add_test(NAME cxx_VirtualInitCtor COMMAND VirtualInitCtor)
endif()
