cmake_minimum_required(VERSION 3.27)
project(libtheo)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(VM)
add_subdirectory(Compiler)
add_subdirectory(CLI)
