cmake_minimum_required(VERSION 3.25)
project(libtheo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

enable_testing()

add_subdirectory(VM)
add_subdirectory(Compiler)
add_subdirectory(CLI)
