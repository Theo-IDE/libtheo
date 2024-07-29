
find_package(BISON)
find_package(FLEX)

set(GENERATED_OUTPUTS
	${CMAKE_CURRENT_SOURCE_DIR}/include/parser.tab.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser.tab.c
)

# if the environment has flex / bison installed, regenerate the parser on build
# the files are also pregenerated in the source tree to avoid having bison and flex
# as dependencies on every platform
if(BISON_FOUND AND FLEX_FOUND)
	add_custom_command(
		OUTPUT ${GENERATED_OUTPUTS}
		COMMAND flex 
			--outfile="./src/lex.yy.c" 
			--header-file="./include/lex.yy.h"  
			--noline
			--nounistd
			"./src/lexer.l"
			DEPENDS src/lexer.l
                COMMAND bison "./src/parser.y" 
			-o "./src/parser.tab.c" 
			--defines="./include/parser.tab.h" 
			--no-lines
			-Wcounterexamples
			DEPENDS src/parser.y
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			
	)
endif()

set(LIBTHEO_C_HEADERS
  include/ast.hpp
  include/parse.hpp
  include/lexer.hpp
  include/lexextra.hpp
  include/gen.hpp
  include/compiler.hpp
)

set(LIBTHEO_C_SOURCES
  src/ast.cpp
  src/parse.cpp
  src/gen.cpp
  src/compiler.cpp
)

set_source_files_properties(src/parser.tab.c PROPERTIES LANGUAGE CXX)
add_library(TheoC ${LIBTHEO_C_HEADERS} ${LIBTHEO_C_SOURCES} ${GENERATED_OUTPUTS})

target_include_directories(TheoC PUBLIC
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/Compiler/include # needed for bison output files to find each other
)

target_link_directories(TheoC PUBLIC
  ${PROJECT_BINARY_DIR}/VM/
)

target_link_libraries(TheoC PUBLIC TheoVM)

add_subdirectory(test)
