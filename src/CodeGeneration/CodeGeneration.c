/*
 * This header file has the logic for generating the code and creating the table
 * entries necessary for the code generation part.
 *
 * We will define several functions to perform the semantic actions that will
 * generate the code.
 */

/*
 * The way that we generate the assembly is that we traverse the tree and fire
 * semantic actions like for the semantic checking phase.
 */
#include "CodeGeneration.h"
#include "../SemanticAnalyzer/SemanticAnalyzer.h"
#include "../parser/AST/AST_SymbolTable.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * This function will set the size and offsets of all of the classes and class
 * functions in the global scope.
 *
 * Because we are hashing the entries when we insert there is no order, so we
 * must make sure that all of the class sizes are created before we begin
 * processing the functions for there size.
 *
 * Note that because there are no pointers in our language we do not have the
 * ability to have members of the same type as the containing class.
 */

/*
 * A register is free if it is true so we will set to true.
 */
bool free_registers[15] = {
    true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true}; // We will keep an array of free
                                               // register flags.
/*
 * This function will return a string that corresponds to the first free
 * register in the register pool.
 *
 */
char *get_first_free_register() {

  unsigned int registerNumber;
  bool found = false;

  for (int i = 0; i < 15; i++) {
    if (free_registers[i]) {
      registerNumber = i + 1;
      found = true;
      free_registers[i] =
          false; // we must set it to false, when we call this function.
      break;
    }
  }

  if (!found) {
    fprintf(stderr, "ERROR ALL REGISTERS ARE OCCUPIED!!!\n");
    return NULL;
  }

  if (registerNumber > 9) {
    char buffer[4];
    snprintf(buffer, sizeof(buffer), "r%d", registerNumber);
    return strdup(buffer);
  } else {
    char buffer[3];
    snprintf(buffer, sizeof(buffer), "r%d", registerNumber);
    return strdup(buffer); // Note that we must call strdup to put the memory on
                           // the heap so it persists after function call.
  }
}

/*
 * This function will create a temporary entry in the symbol table for the
 * temporary results of operations.
 *
 * This function should be called when we encounter operation nodes in the ast
 * traversal. In this case we get the types of the left and right nodes get the
 * return type of the operation and we create a temporary table entry in the
 * symbol table.
 *
 */

TableEntry *create_temporary_entry(node *left, node *right,
                                   const char *operator, ErrorArray * errors) {
  if (!left || !right) {
    return NULL;
  }

  Scope *scopePtr = left->scope;
  if (scopePtr != right->scope) {
    fprintf(stderr, "BIG PROBLEM!!\n");
    return NULL;
  }

  while (scopePtr->type != GLOBAL_SCOPE)
    scopePtr = scopePtr->parentScope;

  TypeInfo leftInfo = get_type_expression(left, errors, scopePtr);

  TableEntry *entry = malloc(sizeof(TableEntry));

  if (!entry) {
    fprintf(stderr, "ERROR - create_temporary_entry(): Could not allocate "
                    "memory exiting.\n");
    return NULL;
  }

  return NULL;
}

/*
 * This function will be used to set the return type size of all of the
 * functions in all of the scopes of the symbol table.
 *
 *
 */

void set_function_return_sizes(Scope *globalScope) {
  if (!globalScope) {
    fprintf(stderr, "ERROR - set_function_return_sizes(): Scope passed is "
                    "null, exiting.\n");
    return;
  }

  for (int i = 0; i < SIZE; i++) {
    if (globalScope->entries[i].tableType == FUNCDEF_ENTRY) {

      unsigned int runningSum = 0;
      Scope *funcScope = globalScope->entries[i].data.funcEntry.scope;

      for (int j = 0; j < SIZE; j++) {
        runningSum += funcScope->entries[j].size;
      }
      globalScope->entries[i].size = runningSum;
    }

    if (globalScope->entries[i].tableType == CLASS_ENTRY) {
      Scope *classScope = globalScope->entries[i].data.classEntry.scope;
      unsigned int runningSum = 0;

      for (int j = 0; j < SIZE; j++) {
        if (classScope->entries[j].tableType == FUNCDEF_ENTRY) {

          Scope *funcScope = classScope->entries[j].data.funcEntry.scope;

          for (int k = 0; k < SIZE; k++) {

            runningSum += funcScope->entries[k].size;
          }
          classScope->entries[j].size = runningSum;
        }
      }
    }
  }
}

/*
 * This function will be used to make a third traversal through the tree, this
 traversal will be used for performing the code generation.
 */
void code_gen_pass(node *root, Scope *globalScope) {

  semantic_stack *stack = init_stack();
  push_node(root, stack);

  // Here we begin to generate the code. e
  while (stack->size > 0) {
    node *current = pop_node(stack);

    if (current->numchildren > 0) {
      for (int i = 0; i < current->numchildren; i++) {
        push_node(current->children[i], stack);
      }
    }
  }
  return;
}

// int main(int argc, char **argv) { return 0; }
