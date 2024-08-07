#ifndef __LIBTHEO_C_PARSER_GENERATOR_LRDEA_HPP_
#define __LIBTHEO_C_PARSER_GENERATOR_LRDEA_HPP_

#include "Compiler/include/ParserGenerator/grammar.hpp"
#include <set>

namespace Theo {
  /* an LR(1) Element */
  struct LRElement {
    // reference to production
    Grammar::Symbol left;
    unsigned int alternative;
    /* parse location (index of the right-side
       element before which the dot is located) */
    unsigned int dot;
    // lookahead (must be a terminal)
    Grammar::Symbol follow;
  };

  bool operator<(const LRElement& l1, const LRElement &ö2);

  /**
   * Constructs a State of the LR(1) prefix DEA;
   * Analogous to the epsilon-hull of the part-set construction;
   * @param I set of LRElements (core elements) to build the hull
   * @param G grammar that the LRElements reference
   * @return  the epsilon-hull of LRElements
   */
  std::set<LRElement> hull (std::set<LRElement> I, Grammar& G);

  /**
   * Returns the state I transitions to with input X;
   * The state transition function of the prefix DEA;
   * @param I input state
   * @param X input symbol
   * @param G grammar that the LRElements reference
   * @return  delta(I, X)
   */
  std::set<LRElement> jump (std::set<LRElement> I, Grammar::Symbol X, Grammar& G);
};

#endif
