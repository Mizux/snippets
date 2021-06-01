set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE _SRCS src/*.hpp src/*.cpp)
add_executable(FindTuple ${_SRCS})

if(BUILD_TESTING)
  add_test(NAME cxx_FindTuple COMMAND FindTuple)
endif()
