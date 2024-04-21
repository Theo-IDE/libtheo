#include <iostream>
#include "Compiler/include/ast.hpp"
#include "Compiler/include/parse.hpp"


int main() {

  std::string math_theo = "\
    # OUT is optional and defaults to x0 if not specified\n\
PROGRAM + IN x0, x1 DO\n\
	LOOP x1 DO\n\
		x0 := x0 + 1\n\
	END\n\
	# x0 implicitly treated as return value\n\
END\n\
\n\
# this is a function where a different variable (x2) is used to return values\n\
PROGRAM * IN x0, x1 OUT x2 DO\n\
	LOOP x1 DO\n\
		# \"+\" is used as infix operator (defined by above function)\n\
		# c-style syntax would look like: x2 := +(x2, x0)\n\
		x2 := x2 + x0 \n\
	END\n\
END\n\
  ";

  std::string main_theo = "\n\
INCLUDE \"math.theo\" # works like copy-and-paste, or include in C/C++\n\
\n\
# language-supported instructions\n\
x0 := 10	# constant assignment is a permissive extension\n\
x1 := 5\n\
x0 := x0 + 1	\n\
\n\
# using programs defined in math.theo\n\
x2 := x0 + x1 	# infix\n\
x2 := +(x0, x1) # c-style\n\
\n\
# note that x3 := x0 + x1 + x2 would NOT be valid syntax; infix calls can only appear top level in expressions to avoid operator-hierachy confusion\n\
x3 := x2 + *(x0, x1) # this works, as infix is top level and second arg is c-style call						  \n\
						  ";
  
  std::string test_code = "\n\
  # This is a Comment, yo\n\
  include \"math.theo\"\n			   \
  x0 := x1 + 10\n\
    x1 := x0 - 1337				\n\
    Loop x1 DO # this is a comment at EOL\n\
      x0 := x0 + 1\n\
      x1 := x1 + 10\n\
    end\n\
    x3 := x1 + 42				\n\
    x4 = +(+(2,3), 3 + 4)\n\
    x5 := first_operand + second_operator\n\
    While +(x5, 5) != 0 Do\n\
      x6 := 420\n\
    End\n\
    MARK: \n\
      x45 := 1 \n\
      IF x45 = 2 THEN GOTO MARK\n\
      Goto MARK\n\
    PROGRAM add IN x1, x2 OUT x2 DO\n\
      LOOP x1 DO\n\
        x2 := x2 + 1\n\
      END\n\
    END\n						\
  ";

  std::map<Theo::FileName, Theo::AST> res = Theo::parse({
      {"syntax_test.theo", test_code},
      {"main.theo", main_theo},
      {"math.theo", math_theo}
    });

  bool all_clear = true;

  for(auto p : res) {
    if(!p.second.parsed_correctly){
      std::cout << "Syntax Errors in \"" << p.first << "\":" << std::endl;
      for(auto e : p.second.errors) {
	std::cout << "Line " << e.line << ": " << e.msg << std::endl;
      }
      all_clear = false;
    } else {
      std::cout << "AST of \"" << p.first << "\"" << std::endl;
      p.second.visualize(std::cout);
    }

    // do not forget to free
    p.second.clear();
  }
  
  return 0;
}
