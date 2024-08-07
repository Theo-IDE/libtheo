
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

# scan test
add_executable(scan_test scan_test.cpp)
add_test(NAME scan_test COMMAND scan_test)

# macro extraction test
add_executable(macro_extraction_test macro_extraction_test.cpp)
add_test(NAME macro_extraction_test COMMAND macro_extraction_test)

# grammar test
add_executable(grammar_test grammar_test.cpp)
add_test(NAME grammar_test COMMAND grammar_test)

# test of the hull of LR(1) Elements
add_executable(hull_test hull_test.cpp)
add_test(NAME hull_test COMMAND hull_test)
