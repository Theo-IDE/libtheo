#ifndef __LIBTHEO_C_PARSE_HPP_
#define __LIBTHEO_C_PARSE_HPP_

#include "Compiler/include/ast.hpp"
#include "Compiler/include/scan.hpp"
#include <map>

namespace Theo {

  /**
   * parse a number of strings;
   */
  AST parse(std::map<FileName, FileContent> files, FileName main);
  
};

#endif
