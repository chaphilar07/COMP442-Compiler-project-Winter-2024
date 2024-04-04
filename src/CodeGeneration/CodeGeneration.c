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
#include "../SemanticAnalyzer/SemanticError.h"
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

unsigned int temp_variable_number = 100;
unsigned int literal_entry_number = 100;

/*
 * This function will get the name of the next available temporary register.
 */
char *get_temp_var_name() {
  char buffer[124];
  snprintf(buffer, sizeof(buffer), "%s%d", "temp", temp_variable_number);
  ++temp_variable_number;

  return strdup(buffer);
}
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
 * This function creates a temporary variable that will be stored in the symbol
 * table.
 */
TableEntry *create_temp_entry(node *astnode, Scope *globalScope,
                              ErrorArray *errors) {

  TableEntry *entry = malloc(sizeof(TableEntry));

  entry->tableType = TEMP_ENTRY;

  TypeInfo leftInfo;
  TypeInfo rightInfo;

  if (astnode->type == relexpr) {
    leftInfo = get_type_expression(astnode->children[2], errors, globalScope);
    rightInfo = get_type_expression(astnode->children[0], errors, globalScope);
  } else {
    leftInfo = get_type_expression(astnode->children[1], errors, globalScope);
    rightInfo = get_type_expression(astnode->children[0], errors, globalScope);
  }

  // If either is a float the result gets promoted to a float.
  if (leftInfo.type == FLOAT_TYPE || rightInfo.type == FLOAT_TYPE)
    entry->size = 8;

  else
    entry->size = 4;

  entry->data.TempVarEntry.name = get_temp_var_name();

  return entry;
}

/*
 * This function will create a litval entry for the symbol table.
 */
char *random_id() {

  int x = literal_entry_number;
  char buffer[124];
  snprintf(buffer, sizeof(buffer), "%s%d", "literal", x);
  literal_entry_number++;

  return strdup(buffer);
}
TableEntry *create_litval_entry(node *astnode) {

  TableEntry *entry = malloc(sizeof(TableEntry));

  if (!entry) {
    fprintf(stderr, "ERROR - create_temp_entry(): Cannot allocate memory for "
                    "new table entry.\n");
    return NULL;
  }
  if (astnode->type != floatnum && astnode->type != intnum) {
    fprintf(stderr, "ERROR - create_temp_entry(): Passed invalid type of node "
                    "to funciton exiting.\n");
    return NULL;
  }

  if (astnode->type == intnum)
    entry->size = 4;
  if (astnode->type == floatnum)
    entry->size = 8;

  entry->tableType = LITVAL_ENTRY;

  entry->data.litval.id = random_id();
  entry->data.litval.value = astnode->value;

  return entry;
}

/*
 * This function will be used to make a third traversal through the tree, this
 traversal will be used for performing the code generation.
 */
void code_gen_pass(node *root, Scope *globalScope) {

  semantic_stack *stack = init_stack();
  push_node(root, stack);
  return;
}

// int main(int argc, char **argv) { return 0; }
