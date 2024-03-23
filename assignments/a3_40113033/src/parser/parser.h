#ifndef PARSER_H
#define PARSER_H
#include "../lexer/Lexer.h"
#include <stdio.h>

int parse(const char *path);
int initLookahead(const char *path);
#endif // !PARSER_H
