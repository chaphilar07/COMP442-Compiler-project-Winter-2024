#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
typedef struct _Token {
  char *lexeme;
  const char *category;
  int valid;
  int line;
} token;

token *get_next_token(token *temp, FILE *source, int *line);
int compile_regex();
void free_regexes();

#endif // !LEXER_H
