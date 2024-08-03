#include "Compiler/include/macro.hpp"
#include "Compiler/include/scan.hpp"
#include <string>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/**
 * extract macros using a minimal recursive-descent parser
 */

using namespace Theo;

struct ExtractionState {
  std::vector<MacroDefinition> incomplete_macros;
  std::vector<ParseError> encountered_errors;
  unsigned int tok_pos;
  std::vector<Theo::Token>& tokens;
  std::vector<Theo::Token> output;
};

Theo::Token::Type lookahead(ExtractionState &es) {
  if (es.tok_pos >= es.tokens.size())
    return Theo::Token::T_EOF;
  return es.tokens[es.tok_pos].t;
}

bool match(ExtractionState &es, Theo::Token::Type expect) {
  if (lookahead(es) != expect){
    int tp = MIN(es.tok_pos, es.tokens.size()-1);
    es.encountered_errors.push_back({
	Theo::ParseError::Type::MACRO_EXTRACT_EXPECT,
	"expected token type '"+ Theo::token_string(expect) + "' but got token of type '" + Theo::token_string(lookahead(es)) + "' with content '" + es.tokens[tp].text + "'",
	es.tokens[tp].file,
	es.tokens[tp].line
      });
    es.tok_pos++;
    return false;
  }
  es.tok_pos++;
  return true;
}

void copy(ExtractionState &es) { es.output.push_back(es.tokens[MIN(es.tok_pos, es.tokens.size()-1)]); }

void advance(ExtractionState &es) { match(es, lookahead(es)); }

void push_macro(ExtractionState &es) {
  MacroDefinition md = {
    .priority = 0,
    .rule = {},
    .content_constraint_token_indices = {},
    .template_token_indices = {},
    .replacement = {}
  };
  es.incomplete_macros.push_back(md);
}

void error(ExtractionState &es, Theo::ParseError::Type t, std::string msg) {
  es.encountered_errors.push_back({
      .t = t,
      .msg = msg,
      .file = es.tokens[MIN(es.tok_pos, es.tokens.size()-1)].file,
      .line = es.tokens[MIN(es.tok_pos, es.tokens.size()-1)].line
    });
}

void push_rule(ExtractionState &es) {
  Token l = es.tokens[es.tok_pos];
  MacroDefinition &md = es.incomplete_macros.back();
  md.rule.push_back(l);

  switch(l.t) {
  case Theo::Token::Type::NV_ID:
  case Theo::Token::Type::ID:
  case Theo::Token::Type::INT:{
    md.content_constraint_token_indices.push_back(md.rule.size()-1);
    break;
  }
  case Theo::Token::Type::PROG_TEMP:
  case Theo::Token::Type::ARGS_TEMP:
  case Theo::Token::Type::ID_TEMP:
  case Theo::Token::Type::INT_TEMP:
  case Theo::Token::Type::VALUE_TEMP: {
    md.template_token_indices.push_back(md.rule.size()-1);
    break;
  }
  default:break;
  }
}

void push_replacement(ExtractionState &es) {
  Token l = es.tokens[es.tok_pos];
  MacroDefinition &md = es.incomplete_macros.back();

  md.replacement.push_back(l);
}

/* grammar */
void D (ExtractionState &es); // grammar rule for macros
void MD(ExtractionState &es);
void A (ExtractionState &es); 

void S(ExtractionState &es) {
  switch(lookahead(es)){
  case Theo::Token::T_EOF: { // S -> EOF
    copy(es);
    advance(es);
    return;
  }
  case Theo::Token::DEFINE: { // S -> "DEFINE" ["PRIORITY" INT] D S
    advance(es);
    push_macro(es);

    if (lookahead(es) == Theo::Token::PRIORITY){
      advance(es);
      if (match(es, Theo::Token::INT)) {
	es.incomplete_macros.back().priority =
	  std::stoi(es.tokens[es.tok_pos-1].text);
      }
    }
    
    D(es);
    S(es);
    return;
  }
  default: {  // S -> ... S
    copy(es);
    advance(es);
    S(es);
    return;
  }
  }
}

void D(ExtractionState &es) {
  switch(lookahead(es)) {
  case Theo::Token::T_EOF: {
    match(es, Theo::Token::Type::AS);
    A(es);
    es.incomplete_macros.pop_back();
    return;
  }
  case Theo::Token::Type::AS: {
    error(es, Theo::ParseError::MACRO_EXTRACT_EMPTY_DEFINE, "empty definition of macro not allowed, ignoring this macro");
    advance(es);
    A(es);
    es.incomplete_macros.pop_back();
    return;
  }
  case Theo::Token::Type::DEFINE: {
    error(es, Theo::ParseError::MACRO_EXTRACT_NESTED, "second 'define' inside macro is invalid, ignoring this token");
    advance(es);
    MD(es);
    return;
  }
  default: { // D -> ... MD
    push_rule(es);
    advance(es);
    MD(es);
    return;
  }
  }
}

void MD(ExtractionState &es) {
  switch(lookahead(es)) {
  case Theo::Token::T_EOF: {
    match(es, Theo::Token::Type::AS);
    A(es);
    es.incomplete_macros.pop_back();
    return;
  }
  case Theo::Token::Type::AS: { // MD -> "AS" A
    advance(es);
    A(es);
    return;
  }
  case Theo::Token::Type::DEFINE: {
    error(es, Theo::ParseError::MACRO_EXTRACT_NESTED, "second 'define' inside macro is invalid, ignoring this token");
    advance(es);
    MD(es);
    return;
  }
  default: {
    push_rule(es);
    advance(es);
    MD(es);
    return;
  }
  }
}

void A(ExtractionState &es) {
  switch(lookahead(es)){
  case Theo::Token::T_EOF: {
    match(es, Theo::Token::Type::END_DEFINE);
    return;
  }
  case Theo::Token::END_DEFINE: { // A -> END_DEFINE
    advance(es);
    return;
  }
  case Theo::Token::Type::DEFINE: {
    error(es, Theo::ParseError::MACRO_EXTRACT_NESTED, "second 'define' inside macro is invalid, ignoring this token");
    advance(es);
    A(es);
    return;
  }
  case Theo::Token::Type::AS: {
    error(es, Theo::ParseError::MACRO_EXTRACT_NESTED, "second 'as' inside macro is invalid, ignoring this token");
    advance(es);
    A(es);
    return;
  }
  default: {
    push_replacement(es);
    advance(es);
    A(es);
    return;
  }
  }
}

Theo::MacroExtractionResult
Theo::extract_macros(std::vector<Theo::Token> tokens) {
  ExtractionState es = {
    .incomplete_macros = {},
    .encountered_errors = {},
    .tok_pos = 0,
    .tokens = tokens,
    .output = {}
  };
  S(es);
  return {
    .errors = es.encountered_errors,
    .tokens = es.output,
    .macros = es.incomplete_macros
  };
}
