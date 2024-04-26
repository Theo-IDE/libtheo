#include <iostream>
#include "Compiler/include/ast.hpp"
#include "Compiler/include/gen.hpp"
#include "Compiler/include/parse.hpp"
#include "VM/include/vm.hpp"

int main() {
  
  std::string test_code = "\n\
PROGRAM + IN x0, x1 OUT x0 DO\n\
  LOOP x1 DO\n\
    x0 := x0 + 1\n\
  END\n\
END\n\
Program * In x1, x2 Do\n\
  LOOP x2 do\n\
    x0 := x0 + x1\n\
  END\n\
END\n\
x0 := 13\n\
x1 := 42\n\
x2 := *(x0 * x1, +(2, 2))\n\
  ";
  std::map<Theo::FileName, Theo::AST> res = Theo::parse({
      {"gen_test.theo", test_code},
    });

  Theo::CodegenResult fin = Theo::gen(res, "gen_test.theo");

  // clear asts
  for(auto a : res) {
    a.second.clear();
  }

  if (!fin.generated_correctly) {
    std::cout << "Errors: " << std::endl;
    for(auto e : fin.errors) {
      std::cout << "[" << (int)e.t<< "]" << "File " << e.file << ", Line " << e.line << ", " << e.message << std::endl;
    }
    return 1;
  }

  fin.code.disassemble(std::cout);

  // run it!
  Theo::VM v(fin.code);

  v.execute();

  Theo::VM::Activation::Data d = v.getActivations().back().getActivationVariables();

  std::cout << "Root vars after execution: " << std::endl;
  for(auto const& ac : d) {
    std::cout << ac.first << " = " << ac.second << std::endl;
  }

  if(d.find("x2") == d.end() || d["x2"] != 2184){
    std::cout << "According to your broken VM (and or Parser/Generator), 13*42*4 = " << d["x2"] << std::endl;
    return 1;
  }
  
  return 0;
}
