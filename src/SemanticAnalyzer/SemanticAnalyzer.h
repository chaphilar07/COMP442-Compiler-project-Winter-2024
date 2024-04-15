#ifndef SEMANTIC_ANALYZER_DOT_H
#define SEMANTIC_ANALYZER_DOT_H

#include "../parser/AST/AST_SymbolTable.h"
#include "SemanticError.h"
int validate_lookup(node *astnode, ErrorArray *errors, Scope *currentScope);
TypeInfo get_type_expression(node *astnode, ErrorArray *arr,
                             Scope *globalScope);
void validate_functioncall(node *astnode, Scope *globalScope, ErrorArray *arr);
void print_type(TypeInfo info);

void second_pass_type_check(node *root, Scope *globalScope, ErrorArray *errors);
#endif
