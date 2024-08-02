#include "Compiler/include/scan.hpp"
#include "Compiler/include/lexer.hpp"

using namespace Theo;

struct Scanner {
  yyscan_t s;
  YY_BUFFER_STATE buf;
  FileName f;
};

Scanner create_scanner(FileContent& in, FileName key) {
  Scanner s;
  yylex_init(&s.s);
  const char *str = in.c_str();
  s.buf = yy_scan_string(str, s.s);
  yyset_lineno(1, s.s);
  yyset_extra(ScannerInfo {key}, s.s);
  s.f = key;
  return s;
}

void cleanup_scanner(Scanner &s) {
  yy_delete_buffer(s.buf, s.s);
  yylex_destroy(s.s);
}

bool exists_scanner(std::vector<Scanner> &ss, FileName key) {
  for(Scanner &s : ss) {
    if (s.f == key)
      return true;
  }
  return false;
}

ScanResult Theo::scan(std::map<FileName, FileContent> files,
                              FileName main) {

  std::vector<ParseError> errors = {};
  std::vector<Token> res = {};

  std::vector<Scanner> lex_stack = {};

  if (files.contains(main))
    lex_stack.push_back(create_scanner(files[main], main));
  else {
    errors.push_back({ParseError::Type::MAIN_FILE_NOT_FOUND, "main file '" + main + "' not found", "-", -1});
  }
  while(!lex_stack.empty()) {
    Scanner &s = lex_stack.back();

    Token t;
    int d = yylex(&t, s.s);

    // EOF for this file
    if (d == 0){
      cleanup_scanner(s);
      lex_stack.pop_back();
      continue;
    }

    if (t.t == Token::Type::INCLUDE) {
      d = yylex(&t, s.s);
      if(d == 0 || t.t != Token::Type::FNAME) {
	errors.push_back({ParseError::Type::EXPECTED_FILENAME, "expected filename after include", s.f, t.line});
	continue;
      }

      FileName nfn = t.text;
      nfn = nfn.substr(1, nfn.size()-2);

      if (!files.contains(nfn)){
	errors.push_back({ParseError::Type::FILE_NOT_FOUND, "file '" + nfn + "' not found", s.f, t.line});
	continue;
      }

      lex_stack.push_back(create_scanner(files[nfn], nfn));
      continue;
    }
    res.push_back(t);
  }

  return {res, errors};
}

