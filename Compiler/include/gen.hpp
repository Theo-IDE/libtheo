#ifndef __LIBTHEO_C_GEN_HPP__
#define __LIBTHEO_C_GEN_HPP__

/**
 * Code Generating Module
 */

#include "VM/include/program.hpp"
#include "Compiler/include/ast.hpp"
#include "Compiler/include/parse.hpp"
#include <map>
#include <string>
#include <vector>
namespace Theo {
  
  struct CodegenResult {
    struct Error {
      enum class Type {
	MALFORMED_AST=0, /* ast malformed, hints at implementation bug*/
	PARSE_ERROR=2, /*error during parse process*/
	UNKNOWN_PROGRAM_NAME=3, /* tried calling an undefined program*/
	ARGSIZE_MISMATCH=4, /*function called with wrong number of arguments*/
	INTERNAL_ERROR=5, /*codegen error, e.g. couldn't backpatch*/
	UNKNOWN_MARK=6, /*GOTO to undefined jump mark*/
      };
      Type t;
      std::string message;
      std::string file;
      int line;
    };

    bool generated_correctly;
    std::vector<Error> errors;
    Theo::Program code;
  };

  CodegenResult gen(Theo::AST);

};

#endif
