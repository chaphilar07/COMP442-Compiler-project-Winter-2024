/*
 * This is the semantic checks file, this file will define functions that will
 * perform some semantic type checks that will be done during the same traversal
 * as the construction of the symbol tables.
 *
 */

#include "SemanticAnalyzer.h"
#include "../parser/AST/AST_SymbolTable.h"
#include "SemanticError.h"
#include <stdio.h>
#include <stdlib.h>
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

/*
 * This function will check for a function's return type, if the return type is
 * not void we must ensure that there is a return statement and that it is the
 * last statement of the function
 * Works correctly.
 */

void check_for_return_statement_function(node *astnode, Scope *globalScope,
                                         ErrorArray *errors) {

  TypeInfo returnInfo;
  returnInfo = get_type_info(astnode);

  int number_of_statements = astnode->children[0]->numchildren;

  node *return_node = NULL;

  for (int i = 0; i < number_of_statements; i++) {
    if (astnode->children[0]->children[i]->type == returnnode) {
      return_node = astnode->children[0]->children[i];
    }
  }

  /*
   * This function only checks if a function has a return type node we do not
   * check if the function has the correct return type.
   */
  if (returnInfo.type == VOID_TYPE && !return_node)
    return; // The return type is void and there is no return statement no error
            // found.

  else if (returnInfo.type != VOID_TYPE && !return_node) {
    // Semantic Error! Non-void return type function must have a return value.

    insert_error(errors,
                 create_error(get_name(astnode), err3100, astnode->line));
  } else if (returnInfo.type != VOID_TYPE && return_node) {
    return; // Ok , we report this elsewhere.
  }
  return;
}
/*
 * This function will traverse the actual symbol tables and checks that the
 * varidable declarations in a class are not of a subclass type, if they are we
 * throw a semantic error.
 */

/*
 * This function traverses the tables and checks for shadowed variables etc, we
 * will use a DFS where we push scopes to traverse inheritance hierarchies for
 * classes.
 *
 *For classes we must check each of the inherited classes to see if they have a
 *variable/function of the same name.
 We will check the scopes of functions and classes.
 For each member of the class we will do a DFS of the inherited classes of the
 current class. If the name appears in one of the super classes we will throw a
 shadowing warning.
 */

void check_for_shadowing_in_member_functions(Scope *globalScope,
                                             ErrorArray *errors) {
  for (int i = 0; i < SIZE; i++) {
    if (globalScope->entries[i].tableType == CLASS_ENTRY) {

      Scope *classScope = globalScope->entries[i].data.classEntry.scope;
      TableEntry classEntry = globalScope->entries[i];
      Scope **inheritedScopes = classEntry.data.classEntry.inheritedScopes;
      int numberOfInheritedScopes = classEntry.data.classEntry.inheritsCount;

      for (int i = 0; i < SIZE; i++) {
        if (classScope->entries[i].tableType == FUNCDEF_ENTRY) {

          Scope *memberFunctionScope =
              classScope->entries[i].data.funcEntry.scope;

          for (int i = 0; i < SIZE; i++) {

            TableEntry functionScopeEntry = memberFunctionScope->entries[i];
            if (functionScopeEntry.tableType == VARIABLE_ENTRY) {
              const char *functionEntryName =
                  functionScopeEntry.data.varEntry.name;

              // Now we check if an inherited class has a variable of the same
              // name in which case we are shadowing.

              TableEntry *correspondingClassScopeEntry =
                  get_entry(classScope, functionEntryName);

              if (correspondingClassScopeEntry->tableType == VARIABLE_ENTRY) {
                insert_error(errors, create_error(functionEntryName, war102,
                                                  classEntry.line));
                break;
              }

              if (numberOfInheritedScopes > 0 && inheritedScopes != NULL) {
                ScopeStack *stack = init_scope_stack();

                for (int i = 0; i < numberOfInheritedScopes; i++) {
                  push_scope(inheritedScopes[i], stack);
                }

                while (stack->size > 0) {
                  Scope *currentScope = pop_scope(stack);

                  TableEntry *entry =
                      get_entry(currentScope, functionEntryName);

                  if (entry->tableType == VARIABLE_ENTRY) {
                    insert_error(errors, create_error(functionEntryName, war102,
                                                      classEntry.line));
                    break;
                  }

                  // Push the scopes onto the stack.
                  TableEntry *currentScopeClassEntry = get_entry(
                      currentScope->parentScope, currentScope->scopeName);

                  for (int i = 0;
                       i <
                       currentScopeClassEntry->data.classEntry.inheritsCount;
                       i++) {
                    push_scope(currentScopeClassEntry->data.classEntry
                                   .inheritedScopes[i],
                               stack);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void check_tables_for_shadowing(Scope *globalScope, ErrorArray *errors) {

  for (int i = 0; i < SIZE; i++) {
    if (globalScope->entries[i].tableType == CLASS_ENTRY) {

      TableEntry currentClassEntry = globalScope->entries[i];
      Scope *currentClassScope =
          globalScope->entries[i]
              .data.classEntry.scope; // Get the scope of the current class.
      Scope **inheritedEntries =
          currentClassEntry.data.classEntry.inheritedScopes;
      int numberOfScopes = currentClassEntry.data.classEntry.inheritsCount;

      if (numberOfScopes <= 0)
        continue;

      for (int i = 0; i < SIZE; i++) {
        TableEntry currentMemberEntry = currentClassScope->entries[i];

        if (numberOfScopes <= 0) {
          continue;
        }

        if (currentMemberEntry.tableType != EMPTY_ENTRY) {

          const char *currentMemberName = NULL;
          int currentLine = currentClassEntry.line;

          EntryType currentMemberEntryType;

          if (currentMemberEntry.tableType == FUNCDEF_ENTRY) {

            currentMemberName = currentMemberEntry.data.funcEntry.name;
            currentMemberEntryType = FUNCDEF_ENTRY;

          } else {

            currentMemberName = currentMemberEntry.data.varEntry.name;
            currentMemberEntryType = VARIABLE_ENTRY;
          }

          // We will use a scope stack to traverse teh inheritance hierarchy.
          ScopeStack *stack = init_scope_stack();

          for (int i = 0; i < numberOfScopes; i++) {
            push_scope(inheritedEntries[i], stack);
          }

          while (stack->size > 0) {
            // here we need to check if the current member variable is being
            // shadowed.
            Scope *currentScope = pop_scope(stack);

            TableEntry *currentEntry =
                get_entry(currentScope, currentMemberName);

            if (currentEntry->tableType == currentMemberEntryType) {
              // We have some kind of shadowing here, we need to check if we
              // have a variable or a function.
              if (currentMemberEntryType == VARIABLE_ENTRY) {
                insert_error(errors, create_error(currentMemberName, war100,
                                                  currentLine));
              }

              if (currentMemberEntryType == FUNCDEF_ENTRY) {
                // We check that the parameters are the same, and that the
                // return type is also the same.
                // We need to check the function parameters of each entry and
                // compare type and number.

                insert_error(errors, create_error(currentMemberName, war101,
                                                  currentLine));

                if (currentEntry->data.funcEntry.numfparams !=
                    currentMemberEntry.data.funcEntry.numfparams) {

                  insert_error(errors, create_error(currentMemberName, err1200,
                                                    currentLine));

                } else {

                  for (int i = 0;
                       i < currentMemberEntry.data.funcEntry.numfparams; i++) {

                    if (!compare_type_info(
                            currentMemberEntry.data.funcEntry.fparamslist[i]
                                ->data.fparamEntry.type,
                            currentEntry->data.funcEntry.fparamslist[i]
                                ->data.fparamEntry.type)) {

                      insert_error(
                          errors,
                          create_error(currentEntry->data.funcEntry.name,
                                       err1201, currentLine));
                    }
                  }
                }
              }
            }

            // Want to push the inherited scopes of this scope, how do we do
            // this.

            TableEntry *classEntry =
                get_entry(currentScope->parentScope, currentScope->scopeName);

            if (classEntry->data.classEntry.inheritsCount > 0 &&
                classEntry->data.classEntry.inheritedScopes != NULL) {

              for (int i = 0; i < classEntry->data.classEntry.inheritsCount;
                   i++) {
                push_scope(classEntry->data.classEntry.inheritedScopes[i],
                           stack);
              }
            }
          }

        } else {
          continue;
        }
      }
    }
  }
  return;
}

/*
 * This function will get the type of an expression that starts with a var.
 * Works corectly, if no type is found we will return a nil error, note we
 * do not have to report the error when this function gets called.
 *
 * Works correctly.
 */

void print_type(TypeInfo info) {
  fprintf(stderr, "Type %s with array dimensions:", info.typeString);
  for (int i = 0; i < info.numberofdims; i++) {
    fprintf(stderr, "[%d]", info.arraydims[i]);
  }
  fprintf(stderr, "\n");
}
void print_type_to_file(TypeInfo info, FILE *file) {
  fprintf(file, "Type %s with array dimensions:", info.typeString);
  for (int i = 0; i < info.numberofdims; i++) {
    fprintf(file, "[%d]", info.arraydims[i]);
  }
  fprintf(file, "\n");
}

int get_dimlist_count(node *astnode) {
  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == dimlist)
      return astnode->children[i]->numchildren;
  }
  return 0;
}

/*
 * This function will return the number of parameters for a given function.
 */
int get_aparams_count(node *astnode) {
  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == aparamslist) {
      return astnode->children[i]->numchildren;
    }
  }
  return 0;
}

/*
 * This function will return the dimlist node from a var or fparam node.
 */
node *get_dimlist_node(node *astnode) {
  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == dimlist)
      return astnode->children[i];
  }

  return NULL;
}

/*
 * This function will return the type information for a variable lookup.
 */

TypeInfo get_type_var(node *astnode, ErrorArray *errors, Scope *globalScope) {

  const char *varName = get_name(astnode);
  Scope *scopePtr =
      NULL; // This will be the start of the scope that we look through.

  // We have to check if the node is a member access.
  if ((astnode->parent->type == dot &&
       astnode->parent->children[0] == astnode) ||
      (astnode->parent->type == dot && astnode->parent->parent->type == dot &&
       astnode->parent->children[1] == astnode)) {

  } else { // If this is not the case we do not have member access and we only
           // need to check the current scope.
  }

  TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
  return temp;
}

TypeInfo get_type_functioncall(node *astnode, ErrorArray *errors,
                               Scope *globalScope) {}

/*
 * This function will return the type of any expression that is passed to
 * it, an expression can be started with any of the following: var,
 * functioncall, moltop, addop, intnum, floatnum. For var, intnum and
 * floatnum, we simply return the type, note that with var we must perform a
 * table lookup and get the type that way, so we must hash the name of the
 * var, and get the entry in the current scope
 */

TypeInfo get_type_expression(node *astnode, ErrorArray *arr,
                             Scope *globalScope) {
  if (astnode->type == notnode) {
    return get_type_expression(astnode->children[0], arr, globalScope);
  } else if (astnode->type == sign) {
    return get_type_expression(
        astnode->children[0], arr,
        globalScope); // We get the type of the expression, the right child of
                      // the sign will always be another expression.
  } else if (astnode->type == intnum) {
    TypeInfo info = {INT_TYPE, "integer", NULL, 0};
    return info;
  } else if (astnode->type == floatnum) {
    TypeInfo info = {FLOAT_TYPE, "float", NULL, 0};
    return info;
  } else if (astnode->type == var) {
    return get_type_var(astnode, arr, astnode->scope);
  } else if (astnode->type == funccall)
    return get_type_functioncall(astnode, astnode->scope);

  // We should refactor this to use recursion rather than do this iteratively.
  else if (astnode->type == dot) {
    // We want to make this part a recursive function.

    node *left = astnode->children[1];
    node *right = astnode->children[0];
    node *parent = astnode->parent;

    if (right->type == dot) {

      if (parent->type == dot) {
        const char *name = get_name(left);
        TypeInfo classInfo =
            get_type_expression(parent->children[1], arr, globalScope);

        EntryType expectedType;

        if (left->type == funccall)
          expectedType = FUNCDEF_ENTRY;
        else
          expectedType = VARIABLE_ENTRY;

        Scope *scopePtr = globalScope;

        while (scopePtr->type != GLOBAL_SCOPE) {
          scopePtr = scopePtr->parentScope;
        }

        TableEntry *classEntry = get_entry(scopePtr, classInfo.typeString);
        if (classEntry->tableType != CLASS_ENTRY) {
          insert_error(arr, create_error(get_name(parent->children[1]), err701,
                                         parent->children[1]->line));
          TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
          return temp; // Error cannot access non class type.
        }

        // Now check that we have a member.

        Scope *classScope = classEntry->data.classEntry.scope;

        TableEntry *memberEntry = get_entry(classScope, name);

        if (memberEntry->tableType != expectedType) {
          insert_error(arr, create_error(get_name(left), err702, left->line));
          TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
          return temp;
        }
      }

      // call the function recursively, if all of the checks are correct if not
      // we will return a dummy value.
      get_type_expression(right, arr, globalScope);
    } else {
      // In this case we return the type of the right child.
      node *left = astnode->children[1];
      node *right = astnode->children[0];
      node *parent = astnode->parent;

      if (parent->type == dot) {
        const char *name = get_name(left);
        TypeInfo classInfo =
            get_type_expression(parent->children[1], arr, globalScope);

        EntryType expectedType;

        if (left->type == funccall)
          expectedType = FUNCDEF_ENTRY;
        else
          expectedType = VARIABLE_ENTRY;

        Scope *scopePtr = globalScope;

        while (scopePtr->type != GLOBAL_SCOPE) {
          scopePtr = scopePtr->parentScope;
        }

        TableEntry *classEntry = get_entry(scopePtr, classInfo.typeString);
        if (classEntry->tableType != CLASS_ENTRY) {
          insert_error(arr, create_error(get_name(parent->children[1]), err701,
                                         parent->children[1]->line));
          TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
          return temp; // Error cannot access non class type.
        }

        // Now check that we have a member.

        Scope *classScope = classEntry->data.classEntry.scope;

        TableEntry *memberEntry = get_entry(classScope, name);

        if (memberEntry->tableType != expectedType) {
          insert_error(arr, create_error(get_name(left), err702, left->line));
          TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
          return temp;
        }
      }
      // We must also check that the right node here is a member of the class of
      // the left!

      const char *name = get_name(right);
      Scope *scopePtr = globalScope;

      while (scopePtr->type != GLOBAL_SCOPE) {
        scopePtr = scopePtr->parentScope;
      }

      EntryType expectedType;

      if (right->type == funccall)
        expectedType = FUNCDEF_ENTRY;
      else
        expectedType = VARIABLE_ENTRY;

      TypeInfo leftInfo = get_type_expression(left, arr, globalScope);
      TableEntry *classEntry = get_entry(scopePtr, leftInfo.typeString);

      if (classEntry->tableType != CLASS_ENTRY) {
        insert_error(arr, create_error(get_name(left), err701, left->line));
        TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
        return temp;
      }

      Scope *classScope = classEntry->data.classEntry.scope;
      TableEntry *memberEntry = get_entry(classScope, name);

      if (memberEntry->tableType != expectedType) {
        insert_error(arr, create_error(get_name(right), err702, right->line));
        TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
        return temp;
      }

      return get_type_expression(right, arr, globalScope);
    }

  } else if (astnode->type == multop || astnode->type == addop ||
             astnode->type == relexpr) {

    // Note that relops will have three children!
    node *left;
    if (astnode->type == relexpr)
      left = astnode->children[2];
    else
      left = astnode->children[1];

    node *right = astnode->children[0];

    TypeInfo info1 = get_type_expression(left, arr, globalScope);
    TypeInfo info2 = get_type_expression(right, arr, globalScope);

    fprintf(stderr, "Comparing type ");
    print_type(info1);
    print_type(info2);

    if (!compare_type_info(info1, info2)) {
      if (astnode->type == relexpr)
        insert_error(arr, create_error(astnode->children[1]->value, err501,
                                       astnode->line));
      else
        insert_error(arr, create_error(astnode->value, err501, astnode->line));
    }

    return get_type_expression(left, arr, globalScope);
  }

  // Could not find the type error.
  TypeInfo info = {NONE_TYPE, NULL, NULL, 0};
  return info;
}

/*
 * This function will determine if a function has been defined somewhere in
 * the source file, throws and error if not, two different kinds of errors,
 * wrong number of parameters and function not defined anywhere in the
 * source file.
 *
 * Note that when this function validates the function being called we can write
 * the assembly code.
 *
 * Note that we can also use this model for the get_type_funccall function
 * instead of what we currently have.
 */
void validate_functioncall(node *astnode, Scope *globalScope, ErrorArray *arr) {

  // This function will check they type and the number of parameters a
  // function gets called with.
  // Either we have a free function call with no dot parent in the tree or we
  // have a function call that is a the child of a dot and is returning a class
  // type variable.
  if (astnode->parent->type != dot ||
      (astnode->parent->type == dot &&
       astnode->parent->children[1] == astnode)) {

    TableEntry *functionEntry = get_entry(globalScope, get_name(astnode));

    // Function not found.
    if (functionEntry->tableType != FUNCDEF_ENTRY) {

      insert_error(arr,
                   create_error(get_name(astnode), err1401, astnode->line));
      return;
    }

    // Get the info for the function parameters.
    int funccallParamsNum = get_aparams_count(astnode);
    int funcdefParamsNum = functionEntry->data.funcEntry.numfparams;

    fprintf(stderr,
            "function %s alled with %d dimensions function defined with %d "
            "dimensions\n",
            get_name(astnode), funccallParamsNum, funcdefParamsNum);

    if (funccallParamsNum != funcdefParamsNum) {
      insert_error(arr,
                   create_error(get_name(astnode), err1402, astnode->line));
      return;
    } else {

      node *dimlistNode = NULL;
      for (int i = 0; i < astnode->numchildren; i++) {
        if (astnode->children[i]->type == aparamslist) {
          dimlistNode = astnode->children[i];
        }
      }
      for (int i = 0; i < funcdefParamsNum; i++) {
        TypeInfo fparamInfo =
            functionEntry->data.funcEntry.fparamslist[i]->data.fparamEntry.type;

        node *currentDim = dimlistNode->children[i];
        TypeInfo argInfo = get_type_expression(currentDim, arr, globalScope);

        fprintf(stderr, "Comparing %s and %s \n",
                get_type_from_enum(fparamInfo.type),
                get_type_from_enum(argInfo.type));
        fprintf(stderr, "The type of the function defs parameter: ");
        print_type(fparamInfo);
        fprintf(stderr, "The type of the function call args: ");
        print_type(argInfo);
        fprintf(stderr, "\n");

        if (!compare_type_info(fparamInfo, argInfo)) {
          insert_error(arr,
                       create_error(get_name(astnode), err1403, astnode->line));
          return;
        }
      }
    }

    // Here we should add the semantic action to write add the function call to
    // the call stack.
  } else if (astnode == astnode->parent->children[0] &&
             astnode->parent->type ==
                 dot) { // If the funcion is a member function call.

    // The function call is a member function call, we must get the class
    // then the function name
    node *classTypeNode = astnode->parent->children[1];
    TypeInfo classTypeInfo =
        get_type_expression(classTypeNode, arr, globalScope);

    TableEntry *classEntry = get_entry(globalScope, classTypeInfo.typeString);

    if (classEntry->tableType == CLASS_ENTRY) {
      TableEntry *functionEntry =
          get_entry(classEntry->data.classEntry.scope, get_name(astnode));

      if (functionEntry->tableType != FUNCDEF_ENTRY) {

        insert_error(arr,
                     create_error(get_name(astnode), err1401, astnode->line));
        return;
      }

      int funccallParamsNum = get_aparams_count(astnode);
      int funcdefParamsNum = functionEntry->data.funcEntry.numfparams;

      fprintf(stderr,
              "function %s called with %d dimensions function defined with %d "
              "dimensions\n",
              get_name(astnode), funccallParamsNum, funcdefParamsNum);
      if (funcdefParamsNum != funccallParamsNum) {
        insert_error(arr,
                     create_error(get_name(astnode), err1402, astnode->line));
        return;
      } else {
        node *dimlistNode = NULL;
        for (int i = 0; i < astnode->numchildren; i++) {
          if (astnode->children[i]->type == aparamslist)
            dimlistNode = astnode->children[i];
        }

        for (int i = 0; i < funccallParamsNum; i++) {

          TypeInfo fparamInfo = functionEntry->data.funcEntry.fparamslist[i]
                                    ->data.fparamEntry.type;
          node *currentDim = dimlistNode->children[i];
          TypeInfo argInfo = get_type_expression(currentDim, arr, globalScope);

          fprintf(stderr, "Comparing %s and %s \n",
                  get_type_from_enum(fparamInfo.type),
                  get_type_from_enum(argInfo.type));
          fprintf(stderr, "The type of the function defs parameter: ");
          print_type(fparamInfo);
          fprintf(stderr, "The type of the function call args: ");
          print_type(argInfo);

          if (!compare_type_info(fparamInfo, argInfo)) {
            insert_error(
                arr, create_error(get_name(astnode), err1403, astnode->line));
            return;
          }
        }
      }
    } else {
      insert_error(
          arr, create_error(classTypeInfo.typeString, err701, astnode->line));
    }
  }
}

/*
 * This function will be used during creation of the symbol table the
 * function will check if a variable being used has been declaerd.
 */

/*
 * This function is used to check if a free function call has been defined
 * and has been called with the right number of parameters, we can also use
 * this on the member function but we must pass the scope of the member
 * function.
 *
 * Note that we will do these checks after the symbol table has been built
 * this means that in our language we can have it that we have functions
 * that are called before they are defined, but we cannot have a function
 * call for a function that is not defined at all.
 */

/*
 * This function will be used to check if an assignment is valid, both sides
 * must be of the same type.
 */
