set(_SRCS main.cpp)
add_executable(SharedFromThis ${_SRCS})

if(BUILD_TESTING)
  add_test(NAME cxx_SharedFromThis COMMAND SharedFromThis)
endif()
