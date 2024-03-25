#ifndef SEMANTIC_ERROR_DOT_H
#define SEMANTIC_ERROR_DOT_H
#include <stdio.h>

typedef enum code {

  err61,
  err62,
  err81,
  err82,
  err83,
  err84,
  war85,
  war86,
  war91,
  war92,
  war93,
  err101,
  err102,
  err103,
  err111,
  err112,
  err113,
  err114,
  err115,
  err121,
  err122,
  err131,
  err132,
  err133,
  err141,
  err151,

  err800,

  err200,
  err202,
  err203,
  err204,
  err205,
  err206,
  err207,

  err501,
  err502,
  err503,

  err701,
  err702,
  err900,
  err901,
  err903,
  err1101,
  err1102,

  ok,

} err_code;

// An error will have two parts a line and a msg for that message, and a code to
// specify the type of the error.
typedef struct semanticerror {

  err_code code;
  const char *msg;
  int line;

} SemanticError;

typedef struct ErrorArray {
  SemanticError *errors;
  int currentSize;
} ErrorArray;

int insert_error(ErrorArray *arr, SemanticError err);
SemanticError create_error(const char *msg, err_code code, int line);
ErrorArray *init_errors();
void print_errors(FILE *out, ErrorArray *arr);
#endif
