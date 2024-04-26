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
	MISSING_INCLUDE=1, /*tried to include non-existent file*/
	PARSE_ERROR=2, /*one of the input ASTs has reported a parse error*/
	UNKNOWN_PROGRAM_NAME=3, /* tried calling an undefined program*/
	ARGSIZE_MISMATCH=4, /*function called with wrong number of arguments*/
	INTERNAL_ERROR=5, /*codegen error, e.g. couldn't backpatch*/
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

  CodegenResult gen(std::map<Theo::FileName, Theo::AST>, std::string main_file);

};

#endif
