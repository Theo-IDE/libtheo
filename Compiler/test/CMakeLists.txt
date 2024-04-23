
include_directories(PUBLIC
  ${PROJECT_SOURCE_DIR}
)

link_directories(PUBLIC ${PROJECT_BINARY_DIR}/Compiler/)

link_libraries(TheoC)

# parse test
add_executable(parse_test parse_test.cpp)
add_test(NAME parse_test COMMAND parse_test)
