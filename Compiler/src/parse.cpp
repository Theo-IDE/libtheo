#include "Compiler/include/parse.hpp"
#include "Compiler/include/lexer.hpp"

using namespace Theo;

AST Theo::parse(std::map<FileName, FileContent> files, FileName main) {
  AST a;
  a.parsed_correctly = false;
  a.root = NULL;
  a.all_allocated_nodes = {};
  // TODO: reimplement parsing
  return a;
}
