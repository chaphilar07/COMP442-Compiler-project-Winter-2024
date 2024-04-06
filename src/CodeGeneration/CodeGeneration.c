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
 * Note we do not use a register pool, we just use the next register and then
 * store any temporary values in the memory of the stack frame.
 *
 */
int current_register_index = 0;

char *get_next_free_register() {

  unsigned int registerNumber;
  registerNumber = (++current_register_index) %
                   12; // Note that we only have a single free register.
  ++registerNumber;

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

/*
 * This function will create a litval entry, these will store the values of
 * literal values in our source, code note that we will reserve memory for the
 * return values of function calls.
 */
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
 *
 * Note that we should refactor this later on to take array dimensions into
 * account.
 *
 * How can we do this in the code?
 *
 * We need to calculate what the offset from the offset of the variable is, then
 * we will use this plus the offset to the variable in the symbol table for the
 * memory access.
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
      if (entry->tableType == VARIABLE_ENTRY) {

        return entry->offset;
      }

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

/*
 * Gets the size of a class.
 */

/*
 * This function will return the size of a function, we use this to push a frame
 * onto the stack.
 */
int get_function_size(node *astnode, void *arr) {
  if (!astnode) {
    return -1;
  }

  ErrorArray *errors = (ErrorArray *)arr;

  const char *name = get_name(astnode);

  if (astnode->parent->type != dot ||
      (astnode->parent->type == dot &&
       astnode == astnode->parent->children[1] &&
       astnode->parent->parent->type != dot)) {

    Scope *scopePtr = astnode->scope;

    while (scopePtr && scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
      if (scopePtr == NULL) {
        fprintf(stderr, "BIG PROBLEM!!!\n");
      }
      if (scopePtr->type == GLOBAL_SCOPE)
        fprintf(stderr, "FOUND GLOBAL SCOPE!\n");
    }

    TableEntry *funcEntry = get_entry(scopePtr, name);

    if (funcEntry->tableType == FUNCDEF_ENTRY) {
      return funcEntry->size;
    }

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

int get_offset(node *astnode, Scope *globalScope, ErrorArray *errors,
               FILE *out) {
  if (astnode->type == var) {
    return get_variable_offset(astnode, errors);
  } else if (astnode->type == intnum || astnode->type == floatnum) {
    const char *name = astnode->entryName;
    TableEntry *entry = get_entry(astnode->scope, name);
    if (entry->tableType == LITVAL_ENTRY)
      return entry->offset;
    else
      return 0;
  } else if (astnode->type == multop || astnode->type == addop ||
             astnode->type == relexpr) {
    const char *name = astnode->entryName;
    TableEntry *entry = get_entry(astnode->scope, name);
    if (entry->tableType == TEMP_ENTRY)
      return entry->offset;
    else
      return 0;
  } else if (astnode->type == funccall) {
    TableEntry *entry = get_entry(astnode->scope, astnode->entryName);
    if (entry->tableType == LITVAL_ENTRY)
      return entry->offset;
    else
      return 0;
  }

  return 0;
}

/*
 *
 * How would we handle an expression that uses
 */
int handle_expression(node *astnode, ErrorArray *errors, FILE *out,
                      Scope *globalScope) {
  if (astnode->type == floatnum || astnode->type == intnum) {

    fprintf(stderr, "HERE\n");
    int offset = get_offset(astnode, globalScope, errors, out);
    const char *registerBeingUsed = get_next_free_register();
    fprintf(out, "addi %s,r0,%s\n", registerBeingUsed,
            astnode->value); // We store the value in a register.
    fprintf(out, "sw %d(r14),%s\n", offset,
            registerBeingUsed); // Store the value that is being stored in the
                                // register in the corresponding offset.

    fprintf(stderr, "HERE\n");
    return offset;

  } else if (astnode->type == multop || astnode->type == addop) {

    int leftOffset =
        handle_expression(astnode->children[1], errors, out, globalScope);
    int rightOffset =
        handle_expression(astnode->children[0], errors, out, globalScope);

    int operationOffset = get_offset(astnode, globalScope, errors, out);

    char operationBuffer[16];

    if (strcmp(astnode->value, "+") == 0)
      snprintf(operationBuffer, sizeof(operationBuffer), "add");
    else if (!strcmp(astnode->value, "*"))
      snprintf(operationBuffer, sizeof(operationBuffer), "mul");
    else if (!strcmp(astnode->value, "-"))
      snprintf(operationBuffer, sizeof(operationBuffer), "sub");
    else if (!strcmp(astnode->value, "/"))
      snprintf(operationBuffer, sizeof(operationBuffer), "div");

    const char *left_register = get_next_free_register();
    const char *right_register = get_next_free_register();
    const char *operation_register = get_next_free_register();

    fprintf(out, "lw %s,%d(r14)\n", left_register,
            leftOffset); // Load the value of the left operand.
    fprintf(out, "lw %s,%d(r14)\n", right_register,
            rightOffset); // Load the value of the right operand.

    fprintf(
        out, "%s %s,%s,%s\n", operationBuffer,
        operation_register, // perform the operation and store it in a buffer.
        right_register, left_register);
    fprintf(
        out, "sw %d(r14),%s\n", operationOffset,
        operation_register); // Store the word in the operations given offset.

    return operationOffset;
  } else if (astnode->type == var) {

    return get_offset(astnode, globalScope, errors, out);

  } else if (astnode->type == funccall) {

    handle_function_call(astnode, errors, globalScope, out);

    int functionOffset = get_offset(astnode, globalScope, errors, out);

    fprintf(out, "sw %d(r14),r13\n", functionOffset);
    return functionOffset;
  }

  return 0;
}

/*
 * We will create this first handle only simple assignments, so we cannot handle
 * things like an exprssion on the RHS of assignment.
 *
 * Note that for an assignment all that we will need is the offset of the value
 * that is being assigned after this we can get that data and store it again
 * using that offset.
 */
void handle_assignment_statement(node *astnode, ErrorArray *errors,
                                 Scope *globalScope, FILE *out) {

  int assignemnt_offset =
      get_offset(astnode->children[1], globalScope, errors, out);
  int value_offset =
      handle_expression(astnode->children[0], errors, out,
                        globalScope); // We need to handle whatever is on the
                                      // left-hand side of the equality.

  const char *register_for_assignment = get_next_free_register();
  const char *register_for_value = get_next_free_register();

  fprintf(out, "lw %s,%d(r14)\n", register_for_value, value_offset);
  fprintf(out, "add %s,r0,%s\n", register_for_assignment, register_for_value);
  fprintf(out, "sw %d(r14),%s\n", assignemnt_offset, register_for_assignment);
}

/*
 * This function will be handleing the function calls as they are traversed.
 *
 * When we arrive at a function call we do the following we store the parameters
 * in registers(1,2,..13 so max 13 parameters).
 *
 * we jump to that functions label.
 *
 * Right now we just want it work with the most basic function call with no
 * function parameters.
 */
void handle_function_call(node *astnode, ErrorArray *errors, Scope *globalScope,
                          FILE *out) {

  const char *name = get_name(astnode);

  int sizeOfFunction = get_function_size(
      astnode,
      errors); // Note that we have a function that gets the size of a function.
  fprintf(out, "addi r14,r14,-%d\n", sizeOfFunction);
  fprintf(out, "jl r15,%s\n", name);
  fprintf(out, "addi r14,r14,%d\n", sizeOfFunction);
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
      fprintf(out, "lw r15, 0(r14)\n");
      fprintf(out, "jr r15\n\n\n");
    }

    if (current->scope)
      current_scope = current->scope;
    if (current->type == funcdef) {

      // We want to store the return address

      const char *name = get_name(current);
      char buffer[64];
      snprintf(buffer, sizeof(buffer), "%s",
               name);                      // Create the label for the function.
      fprintf(out, "align\n%s\n", buffer); // Print the label to the file.
      fprintf(out,
              "sw 0(r14),r15\n"); // We store the stack return address at the
                                  // first byte of the current stack frame.
    }

    // If we get to a function call we must do the following:
    // 1. get the size of the function, increment the topofstackpointer by
    // this amount.
    // 2. store the parameters in free_registers.
    // 3. store the return address at the top of called functions stack frame
    // so it knows where to return to.
    //
    // To do this I will write a function that gets the size of the function
    // call from the node, this will take on a similar structure to our
    // previous functions that will get some data about a function call.
    if (current->type == funccall) {
      handle_function_call(current, errors, globalScope, out);
    }

    if (current->type == assingop) {
      handle_assignment_statement(current, errors, current->scope, out);
    }

    if (current->numchildren > 0) {
      for (int i = 0; i < current->numchildren; i++) {
        push_node(current->children[i], stack);
      }
    }

    if (current->type ==
        returnnode) { // Note that we do NOT DO ANY CHECKING FOR THE CODE GEN!!!

      node *return_value_node = current->children[0];
      int offset =
          handle_expression(return_value_node, errors, out, globalScope);
      fprintf(out, "lw r13,%d(r14)\n", offset);
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
    fprintf(out, "addi r14,r0,topaddr\n");
    fprintf(out, "addi r14,r14,-%d\n", mainFuncSize);
    fprintf(out, "jl r15,main\n");
    fprintf(out, "addi r14,r14,%d\n", mainFuncSize); // Pop of the stack.
    fprintf(out, "hlt\n");
  }

  return;
}

// int main(int argc, char **argv) { return 0; }
//

/*
 * For expressions what we can do is do a dfs of the root of each expression
 * tree, store the values of litvals in there corresponding memory offsets.
 *
 * If we do not have a litval then we have a variable that itself must
 */
