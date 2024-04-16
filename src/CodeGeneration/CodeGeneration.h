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

void print_free_registers();
void free_all_registers();
void reset_index_pointers();
TableEntry *create_litval_entry(node *astnode);

TableEntry *create_temp_entry(node *astnode, Scope *globalScope,
                              ErrorArray *errors);

int get_offset(node *astnode, Scope *globalScope, ErrorArray *errors,
               FILE *out);
void handle_function_call(node *astnode, ErrorArray *errors, Scope *globalScope,
                          FILE *out);
void code_gen_pass(node *root, Scope *globalScope, FILE *out,
                   ErrorArray *errors);
char *get_next_free_register();
void read_subtroutine(FILE *out);
void write_subroutine(FILE *out);

int handle_expression(node *astnode, ErrorArray *errors, FILE *out,
                      Scope *globalScope);
#endif // !CODE_GENERATION_DOT_H
