%{
#define YY_USER_ACTION yylloc->first_line = yylineno;
%}

%{
#include "Compiler/include/lexextra.hpp"
%}

%option reentrant noyywrap yylineno
%option bison-bridge bison-locations
%option extra-type="Theo::yyLexExtra*"

delim [ \t\n\;]
ws {delim}+
name_letter ([a-zA-Z0-9!§\$\%\&\/\?\\\*\+\~\-\<\>\|\_])
digit [0-9]
number \-?{digit}+
id {name_letter}+
program (PROGRAM|program|Program)
in (IN|in|In)
out (OUT|out|Out)
do (\DO|do|Do)
end (END|end|End)
loop (\LOOP|loop|Loop)
while (\WHILE|while|While)
goto (\GOTO|goto|Goto)
if (\IF|if|If)
then (\THEN|then|Then)
include (INCLUDE|include|Include)

%%

\#.* {}
{ws} {}
\"[^\"]*\" {*yylval = yyextra->a->mk(Node::Type::INCLUDE, yylineno, strdup(yytext), NULL, NULL); return(STR);}
{include} {*yylval = NULL; return (KW_INCLUDE);}
{program} {*yylval = NULL; return(KW_PROGRAM);}
{in} {*yylval = NULL; return(KW_IN);}
{out} {*yylval = NULL; return(KW_OUT);}
{do} {*yylval = NULL; return(KW_DO);}
{end} {*yylval = NULL; return(KW_END);}
{loop} {*yylval = NULL; return(KW_LOOP);}
{while} {*yylval = NULL; return(KW_WHILE);}
{goto} {*yylval = NULL; return(KW_GOTO);}
{if} {*yylval = NULL; return(KW_IF);}
{then} {*yylval = NULL; return(KW_THEN);}
\!\={delim}*0 {*yylval = NULL; return(ZERO_INEQUALITY);}
\-/{ws}+{number} {*yylval = NULL; return (SUB_CONSTANT);}
\+/{ws}+{number} {*yylval = NULL;return (ADD_CONSTANT);}
{id}/{delim}*\: {*yylval = yyextra->a->mk(Node::Type::NAME, yylineno, strdup(yytext), NULL, NULL); return(MARK_NAME);}
{id}/{delim}*(\=|\:\=) {*yylval = yyextra->a->mk(Node::Type::NAME, yylineno, strdup(yytext), NULL, NULL); return(NAME_L);}
{number} {*yylval = yyextra->a->mk(Node::Type::NUMBER, yylineno, strdup(yytext), NULL, NULL);  return(CONSTANT);}
{id} {*yylval = yyextra->a->mk(Node::Type::NAME, yylineno, strdup(yytext), NULL, NULL); return(NAME);}
\:\= {*yylval = NULL; return (ASSIGN_OPERATOR);}
\= {*yylval = NULL; return (EQ_OPERATOR);}
\( {*yylval = NULL; return (PAREN_OPEN);}
\) {*yylval = NULL; return (PAREN_CLOSE);}
\, {*yylval = NULL; return (DELIM);}
\: {*yylval = NULL; return (MARK_DELIM);}
%%

