/*
 * This header file will expose the semantic actions that are related to
 *
 */

#ifndef CODE_GENERATION_DOT_H
#define CODE_GENERATION_DOT_H

// We will need the data structures and functions that are exposed from this
// file.
#include "../SemanticAnalyzer/SemanticError.h"
#include "../parser/AST/AST_SymbolTable.h"

void set_function_return_sizes(Scope *globalScope);

TableEntry *create_temporary_entry(node *left, node *right,
                                   const char *operator, ErrorArray * errors);

#endif // !CODE_GENERATION_DOT_H
