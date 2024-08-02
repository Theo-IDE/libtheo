#ifndef __LIBTHEO_C_TOKEN_HPP_
#define __LIBTHEO_C_TOKEN_HPP_

#include <string>

namespace Theo {

  struct Token {
    enum Type {
      ID = 1,
      INT,
      PAREN_CLOSE,
      PAREN_OPEN,
      ARGSEP,
      PROGSEP,
      LABELDEC,
      ASSIGN,
      NEQ_ZERO,
      EQ,
      DO,
      LOOP,
      WHILE,
      GOTO,
      IF,
      THEN,
      STOP,
      END,
      PROGRAM,
      IN,
      OUT,
      INCLUDE,
      FNAME,
      DEFINE,
      AS,
      PRIORITY,
      END_DEFINE,
      PROG_TEMP,
      VALUE_TEMP,
      ID_TEMP,
      INT_TEMP,
      INSERTION,
      TEMP_VAL
    };

    Type t;
    std::string text;
    std::string file;
    int line;

    Token (Type t, std::string text, std::string file, int line) : t(t), text(text), file(file), line(line) {};
  };
  
}

#endif
