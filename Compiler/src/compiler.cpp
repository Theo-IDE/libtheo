#include "Compiler/include/compiler.hpp"
#include "Compiler/include/gen.hpp"

using namespace Theo;

CodegenResult Theo::compile(std::map<FileName, FileContent> files, FileName main) {
  AST intermediate = parse(files, main);
  CodegenResult result = gen(intermediate);

  intermediate.clear();
  
  return result;
}
