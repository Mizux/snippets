set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-enum")

set(_SRCS main.cpp)
add_executable(XMacro ${_SRCS})

if(BUILD_TESTING)
  add_test(NAME cxx_XMacro COMMAND XMacro)
endif()
