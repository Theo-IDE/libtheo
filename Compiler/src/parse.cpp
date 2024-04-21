#include "Compiler/include/parse.hpp"
#include "Compiler/include/lexextra.hpp"
#include "Compiler/include/parser.tab.h"
#include "Compiler/include/lexer.hpp"

using namespace Theo;

std::map<FileName, AST> Theo::parse(std::map<FileName, FileContent> files) {
  std::map<FileName, AST> results = {};
  // for now, just parse main file
  for(auto p : files){
    const char *main_str = p.second.c_str();

    /*setting up flex state*/
    yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE buf;
    buf = yy_scan_string(main_str, scanner);

    // no, flex does not do this by itself, no, i do not no why, PAIN (2 hours spent here)
    // btw it does do this as long as you do not use yy_scan_string
    yyset_lineno(1, scanner);
  
    AST a = {
      .parsed_correctly = true,
    };
    yyLexExtra extra = {
      .a = &a
    };
    yyset_extra(&extra, scanner);

    /*handing control to bison generated parser*/
    yyparse(scanner, &a);

    /*flex cleanup*/
    yy_delete_buffer(buf, scanner);
    yylex_destroy(scanner);

    results[p.first] = a;
  }
  return results;
}
