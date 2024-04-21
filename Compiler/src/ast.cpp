#include "Compiler/include/ast.hpp"

using namespace Theo;

Node *Node::mk(Node::Type t, int line, char *tok, Node *left, Node *right){
  Node *n = new Node();
  n->t = t;
  n->left = left;
  n->right = right;
  n->line = line;
  n->tok = tok;
  return n;
}

Node *AST::mk(Node::Type t, int line, char *tok, Node *left, Node *right) {
  Node *n = Node::mk(t, line, tok, left, right);
  this->all_allocated_nodes.push_back(n);
  return n;
}

void AST::clear() {
  for(auto n : this->all_allocated_nodes)
    delete n;
  this->all_allocated_nodes.clear();
}

#include <cstdlib>
Node::~Node() {
  if(tok != NULL)
    free(tok);
}


static void recurse(Node *n, std::ostream& o, int lvl) {
  if(n == NULL)
    return;
  for(int i = 0; i < lvl; i++){
    o << "|";
    for(int k = 0; k < 3; k++)
      o << ((lvl == i + 1) ? "-" : " ");
  }

  o << "[" << (int)n->t << "] (" << n->line << ") ";
  if(n->tok != NULL)
    o << std::string(n->tok);
  o << std::endl;
  
  recurse(n->left, o, lvl+1);
  recurse(n->right, o, lvl+1);
}

void AST::visualize(std::ostream &o) {
  recurse(this->root, o, 0);
}
