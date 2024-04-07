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

int get_offset(node *astnode, Scope *globalScope, ErrorArray *errors,
               FILE *out);
void handle_function_call(node *astnode, ErrorArray *errors, Scope *globalScope,
                          FILE *out);
void code_gen_pass(node *root, Scope *globalScope, FILE *out,
                   ErrorArray *errors);

void read_subtroutine(FILE *out);
void write_subroutine(FILE *out);
#endif // !CODE_GENERATION_DOT_H
