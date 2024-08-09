#include "Compiler/include/macro.hpp"
#include "Compiler/include/scan.hpp"
#include <algorithm>
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

/* macro application */

struct MacroDetector {

  struct Response {
    // index of first token that was matched
    int location;
    // how many tokens were matched
    int length;
    // the sequence of tokens matched to each macro term
    std::vector<std::vector<Token>> matched;
  };
    
  MacroDefinition md;
    
  MacroDetector(MacroDefinition md){
    this->md = md;
    /* the standard grammar symbols for macro detectors */
    SemanticGrammar<Accumulation> G = SemanticGrammar<Accumulation>();

    auto
      ID = G.createNonTerminal(),
      INT = G.createNonTerminal(),
      VALUE = G.createNonTerminal(),
      ARGS = G.createNonTerminal(),
      P = G.createNonTerminal(),
      STATEMENT = G.createNonTerminal(),
      ATOMIC_P  = G.createNonTerminal(),
      MACRO = G.createNonTerminal();

    auto term = [](int i) -> Grammar::Symbol {return Grammar::Symbol::Terminal(i);};

    auto default_accumulator = [](std::vector<Accumulation> i) -> Accumulation {
      Accumulation a = {{},{}};
      std::for_each(i.begin(), i.end(), [&a](Accumulation &i) -> void {
	a.total_sequence.insert(
				a.total_sequence.end(),
				i.total_sequence.begin(),
				i.total_sequence.end());
      });
      return a;
    };
    
    G.add(ID >>  term(Token::ID),
	  default_accumulator);
    G.add(INT >> term(Token::INT),
	  default_accumulator);
    G.add(VALUE >> ID,
	  default_accumulator);
    G.add(VALUE >> INT,
	  default_accumulator);
    G.add(VALUE >> (term(Token::RUN), ID, term(Token::WITH), ARGS, term(Token::END)),
	  default_accumulator);
    G.add(ARGS >> VALUE,
	  default_accumulator);
    G.add(ARGS >> (ARGS, term(Token::ARGSEP), VALUE),
	  default_accumulator);
    G.add(P >> (P, term(Token::PROGSEP), STATEMENT),
	  default_accumulator);
    G.add(P >> STATEMENT,
	  default_accumulator);
    G.add(STATEMENT >> (ID, term(Token::LABELDEC), ATOMIC_P),
	  default_accumulator);
    G.add(STATEMENT >> ATOMIC_P,
	  default_accumulator);
    G.add(ATOMIC_P >> (ID, term(Token::ASSIGN), VALUE),
	  default_accumulator);
    G.add(ATOMIC_P >> (term(Token::LOOP), ID, term(Token::DO), P, term(Token::END)),
	  default_accumulator);
    G.add(ATOMIC_P >> (term(Token::WHILE), ID, term(Token::NEQ_ZERO), term(Token::DO), P, term(Token::END)),
	  default_accumulator);
    G.add(ATOMIC_P >> (term(Token::GOTO), ID),
	  default_accumulator);
    G.add(ATOMIC_P >> (term(Token::IF), ID, term(Token::EQ), INT, term(Token::THEN), term(Token::GOTO), ID),
	  default_accumulator);
    G.add(ATOMIC_P >> term(Token::STOP),
	  default_accumulator);

    // construct start symbol from macro definition
    std::vector<Grammar::Symbol> sym = {};
    std::for_each(md.rule.begin(), md.rule.end(), [&](Token &t) -> void {
      switch(t.t) {
      case Token::ID_TEMP:
	sym.push_back(ID);
	break;
      case Token::INT_TEMP:
	sym.push_back(INT);
	break;
      case Token::ARGS_TEMP:
	sym.push_back(ARGS);
	break;
      case Token::PROG_TEMP:
	sym.push_back(P);
	break;
      case Token::VALUE_TEMP:
	sym.push_back(VALUE);
	break;
      default:
	sym.push_back(term(t.t));
	break;
      }
    });

    G.add(MACRO >> sym, [](std::vector<Accumulation> i) -> Accumulation {
      Accumulation a = {{}, {}};
      std::for_each(i.begin(), i.end(), [&a](Accumulation &i) -> void {
	a.split_sequence.push_back(i.total_sequence);
	a.total_sequence.insert(
				a.total_sequence.end(),
				i.total_sequence.begin(),
				i.total_sequence.end());
      });
      return a;
    });

    auto transformer = [](Token t) -> Grammar::Symbol {
      return Grammar::Symbol::Terminal(t.t);
    };

    auto creator = [] (Token t) -> Accumulation {
      return {{t}, {{t}}};
    };

    this->parser = LRParser<Accumulation, Token>(G, true, transformer, creator, MACRO,
					   Grammar::Symbol::Terminal(Token::T_EOF));
    this->gen_res = this->parser.generateParseTables();
  }

  std::vector<ParseError> getErrors() {
    std::vector<ParseError> res = {};
    for(auto &g : gen_res) {
      res.push_back(ParseError{
	  ParseError::MACRO_COMPILE_NON_LR,
	  "the macro you defined is non-linear; maybe you have <P> or <ARGS> as your last item; or you have ';' following <P> or ',' following <ARGS>",
	  md.rule.begin()->file,
	  md.rule.begin()->line
	});
    }
    return res;
  }

  std::optional<Response> detect (std::vector<Token> &in); //TODO: detect

private:

  struct Accumulation {
    std::vector<Token> total_sequence;
    std::vector<std::vector<Token>> split_sequence;
  };
  std::vector<LRParser<Accumulation, Token>::GenerationResult> gen_res;    
  LRParser<Accumulation, Token> parser;
};


std::vector<MacroDetector>
get_detectors(std::vector<Theo::MacroDefinition> &defs) {
  std::vector<MacroDetector> res = {};
  for(auto &def : defs) {
    auto det = MacroDetector(def);
    res.push_back(MacroDetector(def));
  }
  return res;
}

Theo::MacroApplicationResult
Theo::apply_macros(std::vector<Theo::Token> input,
             std::vector<Theo::MacroDefinition> &definitions,
             unsigned int passes) {

  std::vector<MacroDetector> detectors = get_detectors(definitions);
  std::vector<MacroDetector> usable = get_detectors(definitions);
  Theo::MacroApplicationResult res = {{},{}};
  // check for errs
  for(auto &detector : detectors) {
    auto lerrs = detector.getErrors();
    res.errors.insert(res.errors.end(), lerrs.begin(), lerrs.end());
    if(lerrs.size() == 0)
      usable.push_back(detector);
  }

  // TODO: actual application
  return res;
}
