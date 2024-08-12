#include <iostream>

#include "Compiler/include/ast.hpp"
#include "Compiler/include/compiler.hpp"
#include "Compiler/include/gen.hpp"
#include "Compiler/include/parse.hpp"
#include "VM/include/vm.hpp"

int main() {
  std::string basemath_code =
      "\
PROGRAM add IN x0, x1 OUT x0 DO\n			\
  WHILE x1 != 0 DO\n					\
    x0 := x0 + 1;\n				\
    x1 := x1 - 1\n				\
  END\n						\
END\n						\
\n\
Program mul In x1, x2 Do\n			\
  start: \n					\
   if x2 = 0 then goto finish;\n			\
   x0 := x0 + x1;\n				\
   x2 := x2 - 1;\n				\
   goto start;\n					\
  finish: NOP\n					\
END\n						\
DEFINE PRIO 10 <V> + <V> AS add($0,$1) END DEFINE\n\
DEFINE PRIO 20 <V> * <V> AS mul($0,$1) END DEFINE\n\
DEFINE PRIO 30 <ID>(<ARGS>) AS RUN $0 WITH $1 END END DEFINE\n\
DEFINE NOP AS _ := 0 END DEFINE\n\
";

  std::string fib_code =
      "\
INCLUDE \"basemath.theo\"\n\
PROGRAM fib IN x2 DO\n	  \
  x0 := 1;\n		  \
  x1 := 1;\n		  \
  x2 := x2 - 2;\n	  \
  LOOP x2 DO \n		  \
    temp := x1;\n	  \
    x1 := x0;\n		  \
    x0 := temp + x0\n	  \
  END\n			  \
END\n\
";

  std::string test_code =
      "\
INCLUDE \"math.theo\"\n				\
//NOTE: double include just overrides\n		\
INCLUDE \"basemath.theo\"\n			\
x0 := 13;\n					\
x1 := 42;\n					\
x2 := mul(x0 * x1, add(2, 2));\n			\
fibres := fib(25)\n				\
  ";

  std::map<Theo::FileName, Theo::FileContent> files = {
      {"gen_test.theo", test_code},
      {"basemath.theo", basemath_code},
      {"math.theo", fib_code}};

  Theo::CodegenResult fin = Theo::compile(files, "gen_test.theo");

  if (!fin.generated_correctly) {
    std::cout << "Errors: " << std::endl;
    for (auto e : fin.errors) {
      std::cout << "[" << (int)e.t << "]"
                << "File " << e.file << ", Line " << e.line << ", " << e.message
                << std::endl;
    }
    return 1;
  }

  fin.code.disassemble(std::cout);

  // run it!
  Theo::VM v(fin.code);

  v.execute();

  Theo::VM::Activation::Data d =
      v.getActivations().back().getActivationVariables();

  std::cout << "Root vars after execution: " << std::endl;
  for (auto const& ac : d) {
    std::cout << ac.first << " = " << ac.second << std::endl;
  }

  if (d["x2"] != 2184) {
    std::cout
        << "According to your broken VM (and or Parser/Generator), 13*42*4 = "
        << d["x2"] << std::endl;
    return 1;
  }

  if (d["fibres"] != 75025) {
    std::cout
        << "According to your broken VM (and/or Parser/Generator), fib(25) = "
        << d["fibres"] << std::endl;
    return 1;
  }

  return 0;
}
