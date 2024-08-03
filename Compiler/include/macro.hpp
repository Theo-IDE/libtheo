#ifndef __LIBTHEO_C_MACRO_HPP_
#define __LIBTHEO_C_MACRO_HPP_

#include <vector>
#include "Compiler/include/token.hpp"
#include "Compiler/include/parse_error.hpp"

namespace Theo {

  struct MacroDefinition {
    /*sequence of tokens to match*/
    std::vector<Token> rule;
    // location of ID and INT tokens in .rule (to match specific var-names)
    std::vector<unsigned int> content_constraint_token_indices;
    // location of grammar tokens in .rule (<P>, <V>, <ID>, <INT>, <ARGS>)
    std::vector<unsigned int> template_token_indices;
    /* sequence of Tokens to replace with */
    std::vector<Token> replacement;
  };

  struct MacroExtractionResult {
    std::vector<Theo::ParseError> errors;
    /* token vector without macro definitions  */
    std::vector<Theo::Token> tokens;
    std::vector<Theo::MacroDefinition> macros;
  };

  /**
   * Extract the macro definitions from a token stream;
   * @param tokens output from scanner
   */
  Theo::MacroExtractionResult extract_macros (std::vector<Theo::Token> tokens);
  
};

#endif
