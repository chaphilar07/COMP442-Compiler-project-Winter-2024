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
int current_register_index = 0;

char *get_next_free_register() {

  unsigned int registerNumber;
  registerNumber = (++current_register_index) % 16;

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
 * This function will get the offset from a variable, this is done by looking at
 * the symbol tables entry.
 *
 * The offset of a variable depends on whether we are accessing a variable that
 * was declared in the current scope, a class or if the variable is being
 * accessed through a member object.
 */
int get_variable_offset(node *astnode, ErrorArray *errors) {
  if (!astnode) {
    fprintf(stderr, "ERROR - get_variable_offset(): Cannot get offset node is "
                    "null, exiting\n");
    return -1;
  }

  const char *name = get_name(astnode);

  // Free variable lookup, we begin search in the current scope.
  if (astnode->parent->type != dot ||
      (astnode->parent->type == dot && astnode->parent->parent->type != dot &&
       astnode->parent->children[1] == astnode)) {

    Scope *scopePtr = astnode->scope;

    TableEntry *entry = get_entry(scopePtr, name);

    // If we find the entry we return the size of the entry?
    if (entry->tableType == VARIABLE_ENTRY) {
      return entry->offset;
    }

    while (scopePtr != NULL && scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;

      entry = get_entry(scopePtr, name);
      if (entry->tableType == VARIABLE_ENTRY)
        return entry->offset;

      if (scopePtr->type == CLASS_SCOPE) {
        // We have to check any inherited scopes as well.
        TableEntry *classEntry =
            get_entry(scopePtr->parentScope, scopePtr->scopeName);

        if (classEntry->tableType == CLASS_ENTRY &&
            classEntry->data.classEntry.inheritsCount > 0 &&
            classEntry->data.classEntry.inheritedScopes) {
          ScopeStack *stack = init_scope_stack();

          for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++) {
            push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
          }

          while (stack->size > 0) {
            Scope *current_scope = pop_scope(stack);

            entry = get_entry(current_scope, name);

            if (entry->tableType == VARIABLE_ENTRY)
              return entry->offset;

            classEntry =
                get_entry(current_scope->parentScope, current_scope->scopeName);
            if (classEntry->tableType == CLASS_ENTRY &&
                classEntry->data.classEntry.inheritsCount > 0 &&
                classEntry->data.classEntry.inheritedScopes) {
              for (int i = 0; i < classEntry->data.classEntry.inheritsCount;
                   i++) {
                push_scope(classEntry->data.classEntry.inheritedScopes[i],
                           stack);
              }
            }
          }
        }
      }
    }

    return -1; // We could not find the variable in any of the offsets.
  } else {
    Scope *scopePtr = astnode->scope;

    while (scopePtr != NULL && scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
    }

    TypeInfo leftInfo;

    if (astnode == astnode->parent->children[1]) {
      leftInfo = get_type_expression(astnode->parent->parent->children[1],
                                     errors, scopePtr);
    } else {
      leftInfo =
          get_type_expression(astnode->parent->children[1], errors, scopePtr);
    }

    TableEntry *classEntry = get_entry(scopePtr, leftInfo.typeString);

    if (classEntry->tableType == CLASS_ENTRY) {
      Scope *classScope = classEntry->data.classEntry.scope;

      TableEntry *entry = get_entry(classScope, name);

      if (entry->tableType == VARIABLE_ENTRY)
        return entry->offset;

      if (classEntry->data.classEntry.inheritsCount > 0 &&
          classEntry->data.classEntry.inheritedScopes) {
        ScopeStack *stack = init_scope_stack();

        for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++)
          push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);

        while (stack->size > 0) {
          Scope *current_scope = pop_scope(stack);

          entry = get_entry(current_scope, name);
          if (entry->tableType == VARIABLE_ENTRY) {
            return entry->offset;
          }

          classEntry =
              get_entry(current_scope->parentScope, current_scope->scopeName);
          if (classEntry->tableType == CLASS_ENTRY &&
              classEntry->data.classEntry.inheritsCount > 0 &&
              classEntry->data.classEntry.inheritedScopes) {

            for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++)
              push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
          }
        }
      }
    }
    return -1;
  }
}

int get_function_size(node *astnode, ErrorArray *errors) {
  if (!astnode) {
    return -1;
  }

  const char *name = get_name(astnode);

  if (astnode->parent->type != dot ||
      (astnode->parent->type == dot &&
       astnode == astnode->parent->children[1] &&
       astnode->parent->parent->type != dot)) {

    return 0;
  } else {
    Scope *scopePtr = astnode->scope;

    while (scopePtr && scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
    }

    TypeInfo leftInfo;

    if (astnode->parent->children[1] == astnode) {
      leftInfo = get_type_expression(astnode->parent->parent->children[1],
                                     errors, scopePtr);
    } else {
      leftInfo =
          get_type_expression(astnode->parent->children[1], errors, scopePtr);
    }
    TableEntry *classEntry = get_entry(scopePtr, leftInfo.typeString);

    if (classEntry->tableType == CLASS_ENTRY) {
      TableEntry *funcEntry =
          get_entry(classEntry->data.classEntry.scope, name);

      if (funcEntry->tableType == FUNCDEF_ENTRY) {
        return funcEntry->size; // Return the size of the function
      }
    }

    if (classEntry->tableType == CLASS_ENTRY &&
        classEntry->data.classEntry.inheritedScopes &&
        classEntry->data.classEntry.inheritsCount > 0) {
      ScopeStack *stack = init_scope_stack();

      while (stack->size > 0) {
        Scope *current = pop_scope(stack);

        TableEntry *funcEntry = get_entry(current, name);

        if (funcEntry->tableType == FUNCDEF_ENTRY) {
          return funcEntry->size;
        }

        classEntry = get_entry(current->parentScope, current->scopeName);

        if (classEntry->tableType == CLASS_ENTRY &&
            classEntry->data.classEntry.inheritsCount &&
            classEntry->data.classEntry.inheritedScopes) {
          for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++) {
            push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
          }
        }
      }
    }
    return 0;
  }
}

/*
 * We will create this first handle only simple assignments, so we cannot handle
 * things like an exprssion on the RHS of assignment.
 */
void handle_assignment_statement(node *astnode, ErrorArray *errors,
                                 Scope *globalScope, FILE *out) {

  node *assigneeNode =
      astnode->children[1]; // The value that we are assigning to.
  node *valueAssignedNode =
      astnode->children[0]; // The value that is being assigned.

  // Right now we will assume that the RHS is a intnum or floatnum not an
  // exprssion although it easily can be.
  int offset = get_variable_offset(assigneeNode, errors);

  // Now we print to the file the value of the offset with the
  // topofstackpointer.
  const char *registerBeingUsed = get_next_free_register();
  const char *next_register = get_next_free_register();
  fprintf(out, "addi\t%s,r0,%s\n", registerBeingUsed,
          valueAssignedNode->value); // We store the value in a register.
  fprintf(out, "addi\t%s,r0,%d\n", next_register, offset);
  fprintf(out, "sw topofstackpointer(%s),%s\n", next_register,
          registerBeingUsed);

  /*
   * 1. store the vaue in a register, the next free register.
   * 2. store the value of the offset in another register.
   * 3.
   */
  return;
}

/*
 * This function will be used to make a third traversal through the tree, this
 traversal will be used for performing the code generation.

 Note that in this function we will create the strings and pass the file
 pointers that are necessary for printing the assembly code.

 We pass a file pointer for the code that we are producing.
 */
void code_gen_pass(node *root, Scope *globalScope, FILE *out,
                   ErrorArray *errors) {

  semantic_stack *stack = init_stack();
  push_node(root, stack);

  // We intitialize the topofstackpointer, should be done for all files.

  fprintf(out, "topofstackpointer res 4\n");
  fprintf(out, "stackregion res 2048\n"); // Reserve 2048 bytes for the function
                                          // call stack region.

  Scope *current_scope = root->scope; // This should be the global scope!
  if (current_scope->type != GLOBAL_SCOPE) {
    fprintf(stderr, "MAJOR PROBLEM!!!!\n");
    exit(0);
  }
  while (stack->size > 0) {

    node *current = pop_node(stack);

    if (current->scope)
      fprintf(stderr, "The current scope is %s and the new nodes scope is %s\n",
              current_scope->scopeName, current->scope->scopeName);

    if (current_scope->type == FUNCTION_SCOPE && current->scope &&
        current->scope != current_scope) {
      // We need to jump back to the correct stack frame location, to do this we
      // get the address that is stored, at the top of the stack frame in the
      // first 4 bytes (word).

      current_scope = current->scope;
      fprintf(out, "lw r15, topofstackpointer(r0)\n");
      fprintf(out, "jr r15\n");
    }

    if (current->scope)
      current_scope = current->scope;
    if (current->type == funcdef) {

      // We want to store the return address

      const char *name = get_name(current);
      char buffer[64];
      snprintf(buffer, sizeof(buffer), "%s",
               name);               // Create the label for the function.
      fprintf(out, "%s\n", buffer); // Print the label to the file.
      fprintf(out,
              "sw topofstackpointer(r0),r15\n"); // We store the return address
                                                 // at the top of the stack.
    }

    if (current->type == multop || current->type == addop ||
        current->type == relexpr) {
    }

    // If we get to a function call we must do the following:
    // 1. get the size of the function, increment the topofstackpointer by this
    // amount.
    // 2. store the parameters in free_registers.
    // 3. store the return address at the top of called functions stack frame so
    // it knows where to return to.
    //
    // To do this I will write a function that gets the size of the function
    // call from the node, this will take on a similar structure to our previous
    // functions that will get some data about a function call.
    if (current->type == funccall) {
      // handle_function_call()
    }

    if (current->type == assingop) {
      handle_assignment_statement(current, errors, current->scope, out);
    }

    if (current->numchildren > 0) {
      for (int i = 0; i < current->numchildren; i++) {
        push_node(current->children[i], stack);
      }
    }
  }

  Scope *scopePtr = globalScope;
  if (scopePtr && scopePtr->type != GLOBAL_SCOPE)
    scopePtr = scopePtr->parentScope;

  // Should be a main funciton if not the program is not exeuctable, just
  // library.
  TableEntry *mainFuncEntry = get_entry(scopePtr, "main");

  if (mainFuncEntry->tableType == FUNCDEF_ENTRY) {

    int mainFuncSize = mainFuncEntry->size;

    fprintf(out, "\nentry\n");
    fprintf(out, "addi r14,r0,-%d\n", mainFuncSize);
    fprintf(out, "sw topofstackpointer(r0),r14\n");
    fprintf(out, "jl r15,main\n");
    fprintf(out, "hlt\n");
  }

  return;
}

// int main(int argc, char **argv) { return 0; }
