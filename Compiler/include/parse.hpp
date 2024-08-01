#ifndef __LIBTHEO_C_PARSE_HPP_
#define __LIBTHEO_C_PARSE_HPP_

#include "Compiler/include/ast.hpp"
#include <map>
#include <string>

namespace Theo {

  typedef std::string FileName, FileContent;

  /**
   * parse a number of strings;
   */
  AST parse(std::map<FileName, FileContent> files, FileName main);
  
};

#endif
