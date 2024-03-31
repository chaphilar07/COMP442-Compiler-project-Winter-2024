#ifndef SEMANTIC_ANALYZER_DOT_H
#define SEMANTIC_ANALYZER_DOT_H

#include "../parser/AST/AST_SymbolTable.h"
#include "SemanticError.h"
int validate_lookup(node *astnode, Scope *currentScope);
void second_pass_type_check(node *root, Scope *globalScope, ErrorArray *errors);
TypeInfo get_type_expression(node *astnode, ErrorArray *arr,
                             Scope *globalScope);

void print_type(TypeInfo info);
#endif
