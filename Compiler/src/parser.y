%define api.pure full

%{
#include <iostream>
#include <cstdint>
#include "Compiler/include/ast.hpp"
using namespace Theo;

#define YYSTYPE Theo::Node*

int yylex (YYSTYPE * yyval_param, struct YYLTYPE * yylloc_param, void * yyscanner);
void yyerror(YYLTYPE *l, void *scanner, Theo::AST *r, const char *const s);
%}

%code requires
{
#include "Compiler/include/ast.hpp"
}


%lex-param {void *scanner}
%parse-param {void *scanner}{Theo::AST *res}
%locations
%define parse.trace
%define parse.error verbose

/*keyword tokens*/
%token KW_PROGRAM KW_IN KW_OUT KW_DO KW_END KW_LOOP KW_WHILE KW_GOTO KW_IF KW_THEN KW_INCLUDE

%token ZERO_INEQUALITY

/* constant,  variable / function name, include string*/
%token CONSTANT NAME STR

/* name followed immediatly by ":", ":" */
%token MARK_NAME MARK_DELIM

/*name followed immediatly by an assign operator (name on the "left side")*/
%token NAME_L 

/* "(", ")", "," and "=" or ":="*/
%token PAREN_OPEN PAREN_CLOSE DELIM ASSIGN_OPERATOR EQ_OPERATOR

%%

start		: program {res->root = $1;}
		;

program		: body_element program  {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, $1, $2);}
		| funcdef      program  {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, $1, $2);}
		| incl_stmt    program  {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, $1, $2);}
		| {$$ = NULL;}
		;

body		: body_element body  {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, $1, $2);}
		| {$$ = NULL;}
		;

body_element	: expression
		| loop_stmt
		| while_stmt
		| goto_stmt
		;

expression	: NAME_L assign_op rvalue {$$ = res->mk(Node::Type::ASSIGN, @1.first_line, NULL, $1, $3);}
		;

assign_op	: ASSIGN_OPERATOR
		| EQ_OPERATOR
		;

rvalue 		: nestable
       		| infix
		;

nestable	: NAME
		| CONSTANT
		| paren_call
		;

paren_call	: NAME PAREN_OPEN optional_arglist PAREN_CLOSE {$$ = res->mk(Node::Type::CALL, @1.first_line, NULL, $1, $3);}
		;

optional_arglist	: arglist
			|
			;

arglist		: rvalue
		| rvalue DELIM arglist {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, $1, $3);}
		;

infix		: nestable NAME nestable {$$ = res->mk(Node::Type::CALL, @2.first_line, NULL, $2, res->mk(Node::Type::SPLIT, @2.first_line, NULL, $1, $3));}
		;

loop_stmt	: KW_LOOP nestable KW_DO body KW_END {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, res->mk(Node::Type::LOOP, @1.first_line, NULL, $2, $4), res->mk(Node::Type::MARK, @5.first_line, NULL, $5, NULL));}
		;

while_stmt	: KW_WHILE nestable opt_ineq KW_DO body KW_END {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, res->mk(Node::Type::WHILE, @1.first_line, NULL, $2, $5), res->mk(Node::Type::MARK, @6.first_line, NULL, $6, NULL));}
		;

opt_ineq	: ZERO_INEQUALITY {$$ = NULL;}
		| {$$ = NULL;}
		;

goto_stmt	: KW_GOTO NAME {$$ = res->mk(Node::Type::GOTO, @1.first_line, NULL, $2, NULL);}
		| KW_IF eq_test KW_THEN KW_GOTO NAME {$$ = res->mk(Node::Type::IF, @1.first_line, NULL, $2, res->mk(Node::Type::GOTO, @4.first_line, NULL, $5, NULL));}
		| MARK_NAME MARK_DELIM {$$ = res->mk(Node::Type::MARK, @1.first_line, NULL, $1, NULL);}
		;

eq_test		: rvalue EQ_OPERATOR rvalue {$$ = res->mk(Node::Type::EQ, @2.first_line, NULL, $1, $3);}
		| NAME_L EQ_OPERATOR rvalue {$$ = res->mk(Node::Type::EQ, @2.first_line, NULL, $1, $3);}
		;

funcdef		: KW_PROGRAM funcname KW_DO body KW_END {$$ = res->mk(Node::Type::PROGRAM, @1.first_line, NULL, $2, res->mk(Node::Type::SPLIT, @1.first_line, NULL, $4, res->mk(Node::Type::MARK, @5.first_line, NULL, $5, NULL)));}
		;

funcname	: NAME progports {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, $1, $2);}
		;

progports	: opt_in opt_out {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, $1, $2);}
		;

opt_in		: KW_IN in {$$ = $2;}
		| {$$ = NULL;}
		;

opt_out		: KW_OUT NAME {$$ = $2;}
		| {$$ = NULL;}
		;

in		: NAME
		| NAME DELIM in {$$ = res->mk(Node::Type::SPLIT, @1.first_line, NULL, $1, $3);}
		;

incl_stmt	: KW_INCLUDE STR {$$ = $2;}
		;

%%

#include "lex.yy.c"

// TODO: actually better error handling
void yyerror(YYLTYPE *l, void *scanner, AST *r, const char *const s){
  r->parsed_correctly = false;
  r->errors.push_back({l->first_line, std::string(s)});
}
