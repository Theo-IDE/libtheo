#ifndef __LIBTHEO_C_PARSE_HPP_
#define __LIBTHEO_C_PARSE_HPP_

#include "Compiler/include/ast.hpp"
#include <map>
#include <string>

namespace Theo {

  typedef std::string FileName, FileContent;

  /**
   * parse a number of strings;
   * When all ASTs of the output have been successfully parsed,
   * the output map can be passed to the code generator;
   */
  std::map<FileName, AST> parse(std::map<FileName, FileContent> files);
  
};

#endif
