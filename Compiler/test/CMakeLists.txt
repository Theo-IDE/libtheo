
include_directories(PUBLIC
  ${PROJECT_SOURCE_DIR}
)

link_directories(PUBLIC ${PROJECT_BINARY_DIR}/Compiler/ ${PROJECT_BINARY_DIR}/VM/)

link_libraries(TheoC TheoVM)

# parse test
add_executable(parse_test parse_test.cpp)
add_test(NAME parse_test COMMAND parse_test)

# codegen test
add_executable(gen_test gen_test.cpp)
add_test(NAME gen_test COMMAND gen_test)
