#ifndef __LIBTHEO_C_SCAN_HPP_
#define __LIBTHEO_C_SCAN_HPP_

#include <string>
#include <map>
#include <vector>

#include "Compiler/include/token.hpp"
#include "Compiler/include/parse_error.hpp"

namespace Theo {
  typedef std::string FileName, FileContent;

  struct ScanResult {
    std::vector<Theo::Token> toks;
    std::vector<ParseError> errors;
  };
  
  /**
   * convert multiple files to one token stream;
   * @param files input files
   * @param main  key of main file
   */
  ScanResult scan (std::map<FileName, FileContent> files, FileName main);
};

#endif
