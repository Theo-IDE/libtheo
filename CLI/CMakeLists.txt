
set(CLI_SOURCES
  cli.cpp
)

add_executable(theo cli.cpp)

target_include_directories(theo PUBLIC
  ${PROJECT_SOURCE_DIR}
)

target_link_directories(theo PUBLIC
  ${PROJECT_BINARY_DIR}/VM/
  ${PROJECT_BINARY_DIR}/Compiler/
)

target_link_libraries(theo PUBLIC TheoVM TheoC)
