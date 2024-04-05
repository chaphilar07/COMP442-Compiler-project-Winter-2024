/*
 * This header file will expose the semantic actions that are related to
 *
 */

#ifndef CODE_GENERATION_DOT_H
#define CODE_GENERATION_DOT_H

// We will need the data structures and functions that are exposed from this
// file.
#include "../SemanticAnalyzer/SemanticAnalyzer.h"
#include "../SemanticAnalyzer/SemanticError.h"
#include "../parser/AST/AST_SymbolTable.h"

TableEntry *create_litval_entry(node *astnode);

TableEntry *create_temp_entry(node *astnode, Scope *globalScope,
                              ErrorArray *errors);

void code_gen_pass(node *root, Scope *globalScope, FILE *out,
                   ErrorArray *errors);
#endif // !CODE_GENERATION_DOT_H
