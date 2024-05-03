#include "Compiler/include/compiler.hpp"
#include "Compiler/include/gen.hpp"

using namespace Theo;

CodegenResult Theo::compile(std::map<FileName, FileContent> files, FileName main) {
  std::map<FileName, AST> intermediate = parse(files);
  CodegenResult result = gen(intermediate, main);

  for(auto a : intermediate)
    a.second.clear();
  
  return result;
}
