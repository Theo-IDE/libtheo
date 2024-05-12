#ifndef _LIBTHEO_COMPILER_AST_HPP_
#define _LIBTHEO_COMPILER_AST_HPP_

#include <iostream>
#include <string>
#include <vector>

namespace Theo {
  // (Node *) is YYSTYPE
  struct Node {
    enum class Type {
      /*a split in the tree, for concatenating nodex*/
      SPLIT = 0,
      /*name: variable or function (left = right = -1)*/
      NAME = 1,
      /*whole number (left = right = -1)*/
      NUMBER = 2,
      /*call node (left = name of func, right = value or tree of values)*/
      CALL = 5,
      /*assign node (left = name, right = value)*/
      ASSIGN = 6,
      /*label (left = right = -1)*/
      LABEL = 7,
      /*loop node (left = value, right = body)*/
      LOOP = 8,
      /*while node (left = value, right = body)*/
      WHILE = 9,
      /*goto node (left = value, right = -1)*/
      GOTO = 10,
      /*if node (left = EQ, right = goto)*/
      IF = 11,
      /*program node (left = tree(name, split(tree of name or null <in>, name or null <out>)), right = body)*/
      PROGRAM = 12,
      /*goto mark (left = name)*/
      MARK = 13,
      /* Equality Test (left = op1, right = op2*/
      EQ = 14,
      /* Include node (tok = filename in quotations)*/
      INCLUDE = 15
    };

    Type t;
    
    char *tok; // c-string token

    int line; // line of the token

    /* children of this node; these pointers are owned by this object*/
    Node *left, *right; 

    ~Node();
    
    static Node *mk(Type t, int line, char *tok, Node *left, Node *right);
  };

  struct SyntaxError {
    int line;
    std::string msg;
  };
  
  struct AST {

    bool parsed_correctly;

    std::vector<SyntaxError> errors;
    
    std::vector<Node*> all_allocated_nodes;
    Node *root;

    // interface used from lex.yy.c and parser.tab.c to create nodes
    Node *mk(Node::Type t, int line, char *tok, Node *left, Node *right);

    /**
     * print a textual visualization of the AST for debug purposes;
     * Example: ast.visualize(std::cout)
     */
    void visualize(std::ostream& output);

    /**
     * deallocate AST;
     */
    void clear();
  };
  
};

#endif
