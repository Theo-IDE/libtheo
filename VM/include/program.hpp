#ifndef _LIBTHEO_VM_PROGRAM_HPP_
#define _LIBTHEO_VM_PROGRAM_HPP_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "VM/include/instr.hpp"
namespace Theo {

struct BreakPoint {
  std::string file;
  int line;
};

};  // namespace Theo

namespace std {

// hashing two breakpoints by xor'ing filename hash and int hash
template <>
struct hash<Theo::BreakPoint> {
  auto operator()(const Theo::BreakPoint &bp) const -> size_t {
    return hash<string>{}(bp.file) ^ hash<int>{}(bp.line);
  }
};

template <>
struct equal_to<Theo::BreakPoint> {
  bool operator()(const Theo::BreakPoint &b1,
                  const Theo::BreakPoint &b2) const {
    return b1.file == b2.file && b1.line == b2.line;
  }
};
};  // namespace std

namespace Theo {

struct Program {
  struct StackMap {
    std::string func_name;
    std::unordered_map<RegisterIndex, std::string> map;
  };

  std::vector<Instruction> code;
  std::vector<StackMap> stack_maps;

  // map from {filename, linenumber} to "bytecode position of potential
  // breakpoint"
  std::unordered_map<BreakPoint, std::vector<ProgramIndex>> potential_breaks;
  // map from "bytecode position of potential breakpoint" to {filename,
  // linenumber}
  std::unordered_map<ProgramIndex, BreakPoint> line_info;

  /* disassemble the program into triplet code*/
  void disassemble(std::ostream &o);

  /**
   * get a list of available breakpoints
   */
  std::unordered_set<BreakPoint> getAvailableBreakpoints();
};

}  // namespace Theo

#endif
