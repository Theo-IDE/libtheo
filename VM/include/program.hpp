#ifndef _LIBTHEO_VM_PROGRAM_HPP_
#define _LIBTHEO_VM_PROGRAM_HPP_

#include <vector>
#include <unordered_map>
#include <string>
#include "VM/include/instr.hpp"
namespace Theo {

  struct Program {
    typedef int LineBreak;
    typedef std::unordered_map<RegisterIndex, std::string> StackMap;
    
    std::vector<Instruction> code;
    std::vector<StackMap> stack_maps;
    std::unordered_map<LineBreak, ProgramIndex> potential_breaks;
    std::unordered_map<ProgramIndex, LineBreak> line_info;
  };
  
}

#endif
