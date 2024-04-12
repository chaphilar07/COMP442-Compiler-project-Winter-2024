#ifndef PARSER_H
#define PARSER_H

#include "../lexer/Lexer.h"
#include "AST/AST_SymbolTable.h"
#include <stdio.h>

node *parse(const char *path);
int initLookahead(const char *path);
#endif // !PARSER_H
