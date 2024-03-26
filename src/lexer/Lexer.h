#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>

/*
 * This enum we will use to assign to the category, instead of using strings
 * will make things easier for us.
 */
typedef enum tokenType {
  inlinecmt,
  blockcmt,
  arrow,
  lcubr,
  rcubr,
  lpar,
  rpar,
  rsqbr,
  lsbr,
  comma,
  dot_lex,
  assign,
  equals,
  identifier_lex,
  intlit,
  floatlit,
  gt,
  lt,
  geq,
  leq,
  neq,
  not_lex,
  indvalid_identifier,
  colon,
  semi,
  or_lex,
  and_lex,
  mult_lex,
  div_lex,
  plus_lex,
  minus_lex,

} tokenType;

typedef struct _Token {
  char *lexeme;
  const char *category;
  int valid;
  int line;
} token;

token *get_next_token(token *temp, FILE *source, int *line);
int compile_regex();
void free_regexes();
int is_keyword(char *);
#endif // !LEXER_H
