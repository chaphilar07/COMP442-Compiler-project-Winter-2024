/*
 * This is the semantic checks file, this file will define functions that will
 * perform some semantic type checks that will be done during the same traversal
 * as the construction of the symbol tables.
 *
 */

#include "../parser/AST/AST_SymbolTable.h"
#include "SemanticError.h"
#include <stdio.h>
#include <string.h>

/*
 *Note we will directly include inherited class members in the scope of the
 *class that is inheriting, this will simplify our life at the cost of extra
 *memory consumption.

 Note that we must take into account inherited classes as well, if we have a
 variable that is in a class scope that is inheriting.

 We will make a seperate function for checking for function calls.
 Note that we can do function declarations after we have built the symbol table,
 this is becuase we want to be able to call a function before it is defined so
we can have implicit function calls
 */

int validate_lookup(node *astnode, Scope *currentScope) {
  if (!astnode) {
    fprintf(stderr,
            "ERROR - validate_lookup(): Node passed is null exiting\n'");
    return -1;
  }
  if (!currentScope) {

    fprintf(stderr,
            "ERROR - validate_lookup(): Scope passed is null exiting\n'");
    return -1;
  }

  // What this function needs to do is check if the current scope has a
  // corresponding entry for the variable. we should be able to call this
  // recursively until we either reach a scope with the variable OR we are in
  // the global scope.

  // Check the current scope.
  const char *name = get_name(astnode);
  TableEntry *entry = get_entry(currentScope, name);

  // Check if the entry is declared in the current scope.
  if ((entry->tableType == VARIABLE_ENTRY ||
       entry->tableType == FPARAM_ENTRY)) {
    fprintf(stderr, "FOUND ENTRY %s IN SCOPE %s \n", name,
            currentScope->scopeName);
    return ok;
  }

  Scope *scopePtr = currentScope;

  // Now we must check all of the parent scopes for the variable declaration.

  while ((scopePtr = scopePtr->parentScope)->type != GLOBAL_SCOPE) {
    // If our variable is used within the scope of a class we can check all the
    // way up the hierarchy.

    fprintf(stderr, "Entering scope %s ...\n", scopePtr->scopeName);

    if (scopePtr->type == CLASS_SCOPE) {
      // We check the current class scope.

      entry = get_entry(scopePtr, name);

      if (entry->tableType == VARIABLE_ENTRY &&
          strcmp(name, entry->data.varEntry.name) == 0) {
        fprintf(stderr, "FOUND %s IN SCOPE %s\n", name, scopePtr->scopeName);
        return ok;
      }

      TableEntry *classEntry =
          get_entry(scopePtr->parentScope, scopePtr->scopeName);

      fprintf(stderr, "Checking inherited classes...\n");
      // We check if classEntry inherits.
      ScopeStack *stack =
          init_scope_stack(); // make memory for a new scope stack.

      if (classEntry->data.classEntry.inheritsCount > 0 &&
          classEntry->data.classEntry.inheritedScopes != NULL) {

        fprintf(stderr, "CLASS %s inherits %d classes, must check ",
                classEntry->data.classEntry.name,
                classEntry->data.classEntry.inheritsCount);

        for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++) {
          fprintf(stderr, "%s ",
                  classEntry->data.classEntry.inheritedScopes[i]->scopeName);

          push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
          fprintf(stderr, "Top of the stack %s \n",
                  peek_scope(stack)->scopeName);
        }
        fprintf(stderr, "\n");
        fprintf(stderr, "%s\n", peek_scope(stack)->scopeName);
      }

      // Using this we can check if a variable has been declared in any of the
      // super scopes.
      // We basically do a DFS through the inherited scopes.
      while (stack->size > 0) {
        Scope *current = pop_scope(stack);
        fprintf(stderr, "Checking inherited class %s for entry %s ... \n",
                current->scopeName, name);
        entry = get_entry(current, name);

        if (entry->tableType == VARIABLE_ENTRY &&
            strcmp(entry->data.varEntry.name, name) == 0) {
          fprintf(stderr, "FOUND ENTRY %s in SCOPE %s .\n", name,
                  current->scopeName);
          return ok;
        }

        classEntry = get_entry(current->parentScope, current->scopeName);
        if (classEntry->data.classEntry.inheritsCount > 0 &&
            classEntry->data.classEntry.inheritedScopes != NULL) {
          for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++) {
            push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
          }
        }
      }
    }
  }

  return err111;
}

/*
 * This function is used to check if a free function call has been defined and
 * has been called with the right number of parameters, we can also use this on
 * the member function but we must pass the scope of the member function.
 *
 * Note that we will do these checks after the symbol table has been built this
 * means that in our language we can have it that we have functions that are
 * called before they are defined, but we cannot have a function call for a
 * function that is not defined at all.
 */

int check_functioncall(node *astnode, Scope *currentScope) {
  if (!astnode) {
    fprintf(stderr, "ERROR - check_functioncall(): AST node is null exiting\n");
    return -1;
  }
  if (!currentScope) {
    fprintf(stderr, "ERROR - check_functioncall(): Scope is null exiting.\n");
    return -1;
  }

  // Here we need to check if the function is defined, then we have to check
  // that the type and number of the parameters are correct.
  // Member function calls can only be called on objects of those types free
  // functions can be called at any time.
  // A function will always be called within the scope of a function, so we must
  // check the global scope, for that function definition. We will assume that
  // the function call that is passed is the correct scope.

  TableEntry *funcEntry = get_entry(currentScope, get_name(astnode));

  // Now we need to check
  // 1. the type of the entry it must be a funcdef
  // 3. the fparams of the function are the same.

  if (funcEntry->tableType != FUNCDEF_ENTRY) {
    return err202; // Function has not been defined.
  }

  // Now check the parameters, must be the same as the entry.
  // TableEntry **fparams = get_fparams_list(astnode, currentScope);
  int fparamsCount = get_fparam_count(astnode);

  // If the number of parameters differs.
  if (fparamsCount != funcEntry->data.funcEntry.numfparams) {
    return err203;
  }
  // Now we must check that they are actually the same types of parameters, note
  // that we can have expressions etc be passed as parameters to functions.

  return ok;
}

/*
 * This function checks the type of an AST node, this is used when we have a
 * declaration of a variable, if that variable is not a of a type that has been
 * defined program.
 */
int check_type(node *astnode) { return ok; }

/*
 * This will check a dot, it will check if both sides of the dot are correct,
 * eg. LHS = class type, RHS = member of the class type.
 *
 * All classes are declared in the global scope, cannot have class in any other
 * scope.
 */
int check_dot(node *astnode, Scope *globalScope) {
  if (!astnode) {
    fprintf(stderr, "ERROR - check_dot(): AST node has not been allocated.\n");
    return -1;
  }

  return ok;
}

/*
 * This function will check operators like arithmetic operators and relational
 * operators.
 */
int check_operator(node *astnode, Scope *currentScope) { return ok; }

/*
 * This function will be used to check if an assignment is valid, both sides
 * must be of the same type.
 */

void second_pass_type_check(node *root, Scope *globalScope,
                            ErrorArray *errors) {

  semantic_stack *stack = init_stack();

  push_node(root, stack);

  while (stack->size > 0) {
    node *current = pop_node(stack);

    /*
     * We do type check on declaration if the type of a declaration is not known
     * we throw semantic error.
     */

    if (current->type == vardecl && get_type_enum(current) == ID_TYPE) {

      const char *typeName = get_type_string(current);
      EntryType classType = get_entry(globalScope, typeName)->tableType;

      if (classType != CLASS_ENTRY) {
        fprintf(stderr, "Class has not been defined cannot declare a variable "
                        "of this type.\n");
        insert_error(errors,
                     create_error(get_name(current), err205, current->line));
      }
    }

    if (current->type == fparam && get_type_enum(current) == ID_TYPE) {

      const char *typeName = get_type_string(current);
      EntryType classType = get_entry(globalScope, typeName)->tableType;

      if (classType != CLASS_ENTRY) {
        fprintf(
            stderr,
            "Class has not been defined cannot declare a function parameter "
            "of this type.\n");
        insert_error(errors,
                     create_error(get_name(current), err206, current->line));
      }
    }

    if (current->type == funcdef && get_type_enum(current) == ID_TYPE) {
      const char *typeName = get_type_string(current);
      EntryType classType = get_entry(globalScope, typeName)->tableType;

      if (classType != CLASS_ENTRY) {
        fprintf(stderr, "Class has not been defined cannot declare a function "
                        "to have a return type "
                        "of this type.\n");
        insert_error(errors,
                     create_error(get_name(current), err207, current->line));
      }
    } else if (current->type == returnnode) {
      // We need to check that the type that is inside of the return is of the
      // same type as the return type of the function.

    } else {
      for (int i = 0; i < current->numchildren; i++)
        push_node(current->children[i], stack);
    }
  }
}
