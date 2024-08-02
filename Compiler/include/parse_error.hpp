#ifndef __LIBTHEO_C_PARSE_ERROR_HPP_
#define __LIBTHEO_C_PARSE_ERROR_HPP_

#include <string>

namespace Theo {

  struct ParseError {
    enum Type {
      MAIN_FILE_NOT_FOUND,
      EXPECTED_FILENAME,
      FILE_NOT_FOUND
    };
    Type t;
    std::string msg;
    std::string file;
    int line;
  };
  
};

#endif
