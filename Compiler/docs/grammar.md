# Grammar Specification
This file specifies the grammar that is to be implemented by libTheo using BNF (adhering to the specification found at https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form).
The grammar mostly adheres to the definitions by Uwe Schöning, with a few permissive extensions to make using the language less masochistic.

Changes made:
- semicolons optional
- "=" available as alias for ":="
- LOOP, WHILE, GOTO statements accept constants and results of functions as input
- "!= 0" in WHILE statements optional
- include statement added
- functions (definable with PROGRAM) added
- calling functions (programs) added
	- with c-like syntax
	- with infix syntax

The starting symbol is `P`. `C` denotes a constant signed integer `(-?[0-9]+)`, `X` an alphanumeric variable name `([a-Z][a-Z0-9_]*)`, `S` any string not containing tabs, spaces or newlines, `$` denotes newline characters, `*` any string.

```BNF
<P> ::= (<body_element> | <program_definition> | <include_statement>) <optional_semicolon> (<P> | "")

<optional_semicolon> ::= "" | ";"

<body_element> ::= <expression> | <loop_statement> | <while_statement> | <goto_statement> | <comment>

<body> ::= "" | (<body_element> ("" | (<optional_semicolon> <body>)))

<include_statement> ::= ("INCLUDE" | "include" | "Include") <S>

<comment> ::= "#" * $
```
## Expressions
```BNF
<expression> ::= <X> ("=" | ":=") <addition> | <expression_term> | <infix_call>)

<expresion_term> ::= <c_style_call> | <X> | <C>

<addition> ::= <X> ("+" | "-") <C>

<infix_call> ::= <expression_term> <S> <expression_term>

<c_style_call> ::= <S> "(" <arglist> ")"

<arglist> ::= "" | <expression_term> ("" | ("," <arglist>)))
```
## Constructs
```BNF
<end_kw> = "END" | "end" | "End"

<loop_statement> ::= ("LOOP" | "loop" | "Loop") <expression_term> ("DO" | "do" | "Do") <body> <end_kw>

<while_statement> ::= ("WHILE" | "while" | "While") <expression_term> ("" | ("!=" "0")) ("DO" | "do" | "Do") <body> <end_kw>

<goto_statement> ::= <unconditional_goto> | <conditional_goto> | ("STOP" | "stop" | "Stop")

<unconditional_goto> ::= <goto_kw> <mark>

<conditional_goto> ::= ("IF | "if" | "If") <expression_term> "=" <expression_term> ("THEN" | "then" | "Then") <goto_kw> <mark>

<goto_kw> ::= "GOTO" | "goto" | "Goto"

<mark> ::= <X> ":"
```
## Programs (Functions)
```BNF
<program_definition> ::= ("PROGRAM" | "program" | "Program") <S> ("IN" | "in" | "In") <argdef_list> <optional_out> ("DO" | "do" | "Do") <body> <end_kw>

<argdef_list> ::= "" | (<X> ("" | ("," <argdef_list>)))

<optional_out> ::= "" | (("OUT" | "out" | "Out") <X>)
```
# An Example Program
Here is an example program made up of two files: math.theo and main.theo;
It only uses the LOOP-features but demonstrates the most important deviations from the "standard" LOOP syntax (Program definition, Include statements and Calls).

## math.theo
```
# OUT is optional and defaults to x0 if not specified
PROGRAM + IN x0, x1 DO
	LOOP x1 DO
		x0 := x0 + 1
	END
	# x0 implicitly treated as return value
END

# this is a function where a different variable (x2) is used to return values
PROGRAM * IN x0, x1 OUT x2 DO
	LOOP x1 DO
		# "+" is used as infix operator (defined by above function)
		# c-style syntax would look like: x2 := +(x2, x0)
		x2 := x2 + x0 
	END
END
```

## main.theo
```
INCLUDE math.theo # works like copy-and-paste, or include in C/C++

# language-supported instructions
x0 := 10	# constant assignment is a permissive extension
x1 := 5
x0 := x0 + 1	

# using programs defined in math.theo
x2 := x0 + x1 	# infix
x2 := +(x0, x1) # c-style

# note that x3 := x0 + x1 + x2 would NOT be valid syntax; infix calls can only appear top level in expressions to avoid operator-hierachy confusion
x3 := x2 + *(x0, x1) # this works, as infix is top level and second arg is c-style call
```
