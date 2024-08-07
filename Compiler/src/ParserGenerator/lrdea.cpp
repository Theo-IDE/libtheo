#include "Compiler/include/ParserGenerator/lrdea.hpp"
#include <set>
#include <vector>

using namespace Theo;

bool Theo::operator<(const LRElement &l1, const LRElement &l2) {
  if(l1.left < l2.left)
    return true;
  if(l2.left < l1.left)
    return false;
  if(l1.alternative < l2.alternative)
    return true;
  if(l2.alternative < l1.alternative)
    return false;
  if(l1.dot < l2.dot)
    return true;
  if(l2.dot < l1.dot)
    return false;
  return l1.follow < l2.follow;
}

static Grammar::Alternative fetch_right(const LRElement &e, Grammar &G) {
  return G.right_sides[e.left][e.alternative];
}

static Grammar::Symbol get_before(Grammar::Alternative &a, const LRElement &e) {
  if (a.size() == 0)
    return Grammar::Symbol::Epsilon();
  return a[e.dot];
}

#include <iostream>
static std::vector<Grammar::Symbol> get_follow_string(Grammar::Alternative &a,
                                                      const LRElement &e) {
  std::vector<Grammar::Symbol> res = {};
  for(unsigned int i = e.dot + 1; i < a.size(); i++) {
    res.push_back(a[i]);
  }
  res.push_back(e.follow);
  return res;
}


std::set<LRElement> Theo::hull(std::set<LRElement> I, Grammar& G) {
  bool changed = true;
  while(changed) {
    changed = false;
    for (const LRElement &elem : I) {
      Grammar::Alternative a = fetch_right(elem, G);
      Grammar::Symbol expecting = get_before(a, elem);

      if (expecting.t != Grammar::Symbol::NON_TERMINAL)
	continue;

      std::set<Grammar::Symbol> lookaheads = G.first(get_follow_string(a, elem));

      std::vector<Grammar::Alternative> &Bs = G.right_sides[expecting];

      for (unsigned int right_index = 0; right_index < Bs.size(); right_index++) {
	for (const Grammar::Symbol& la : lookaheads) {
	  LRElement new_elem = {
	    expecting,
	    right_index,
	    0,
	    la
	  };

	  if (!I.contains(new_elem)) {
	    changed = true;
	    I.insert(new_elem);
	    break;
	  }
	}
	if (changed)
	  break;
      }
      if (changed)
	break;
    }
  }
  return I;
}
