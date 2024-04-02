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
 * This function will check for a function's return type, if the return type is
 * not void we must ensure that there is a return statement and that it is the
 * last statement of the function*/

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
void check_tables_for_circular_inheritance(Scope *globalScope,
                                           ErrorArray *arr) {
  if (!globalScope) {
    return;
  }
  if (!arr) {
    return;
  }

  Scope *scopePtr = globalScope;

  // In the global scope we will only have class and function entries.
  for (int i = 0; i < SIZE; i++) {
    if (scopePtr->entries[i].tableType == CLASS_ENTRY) {
      TableEntry classEntry = scopePtr->entries[i];
      Scope *classScope = classEntry.data.classEntry.scope;

      for (int i = 0; i < SIZE; i++) {
        // Get the return type information of each entry and check if it is a
        // class type.

        TypeInfo memberInfo;
        TableEntry memberEntry = classScope->entries[i];

        if (memberEntry.tableType == VARIABLE_ENTRY)
          memberInfo = memberEntry.data.varEntry.type;
        else
          memberInfo = memberEntry.data.funcEntry.returnType;

        if (memberInfo.type == ID_TYPE) {

          const char *classTypeString = memberInfo.typeString;

          for (int i = 0; i < SIZE; i++) {

            TableEntry classEntryCompare = globalScope->entries[i];

            if (classEntryCompare.tableType == CLASS_ENTRY) {
              for (int i = 0;
                   i < classEntryCompare.data.classEntry.inheritsCount; i++) {
                if (!strcmp(classTypeString, classEntryCompare.data.classEntry
                                                 .inheritedScopes[i]
                                                 ->scopeName)) {
                  fprintf(stderr,
                          "Inserting error circular class dependency.\n");
                  insert_error(arr,
                               create_error(classEntry.data.classEntry.name,
                                            err0001, -1));
                }
              }
            }
          }
        }
      }
    }
  }
}

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
 * This function will get the type of a variable that is being used, should be
 * called almost whenever we encounter a var type node in the AST traversal for
 * semantic checks.
 *
 * We take into account when we are accessing a variable directly or through an
 * object of a class type.
 */
TypeInfo get_type_var(node *astnode, ErrorArray *arr, Scope *globalScope) {

  const char *name = get_name(astnode);
  int varDimsCount = get_dimlist_count(astnode);
  node *dimlistNode = get_dimlist_node(astnode);

  // If we have a free variable, eg not being accessed through a class.
  if (astnode->parent->type != dot ||
      (astnode->parent->type == dot &&
       astnode == astnode->parent->children[1] &&
       astnode->parent->parent->type != dot)) {

    if (astnode->scope == NULL) {
      fprintf(stderr,
              "ERROR - node %s, line %d does not have a scope !, exiting\n",
              name, astnode->line);
      TypeInfo info = {NONE_TYPE, "nil", NULL, 0};
      return info;
    }

    TableEntry *entry =
        get_entry(astnode->scope, name); // Get the corresponding entry for the
                                         // astnode in the current scope.

    if (entry->tableType == VARIABLE_ENTRY ||
        entry->tableType == FPARAM_ENTRY) {
      TypeInfo variableTypeInfo =
          entry->data.varEntry.type; // Get the type enum from the symbol table.
      TypeInfo curVarTypeInfo;

      // Checking the type of the variable.
      if (variableTypeInfo.numberofdims >= varDimsCount) {
        for (int i = 0; i < dimlistNode->numchildren; i++) {

          TypeInfo index =
              get_type_expression(dimlistNode->children[i], arr, globalScope);
          fprintf(stderr, "VARIABLE %s CALLED WITH ARRAY INDEX %d WITH TYPE: ",
                  get_name(astnode), i);

          if (index.type != INT_TYPE ||
              (index.type == INT_TYPE && index.numberofdims != 0)) {
            fprintf(stderr, "ERROR! VARIABLE CALLED WITH NON INTEGER TYPE! \n");
            insert_error(
                arr, create_error(get_name(astnode), err2100, astnode->line));
          }
        }
      }

      if (variableTypeInfo.numberofdims == varDimsCount) {
        curVarTypeInfo.numberofdims =
            0; // Because we are accessing the direct type not an array type.
        curVarTypeInfo.arraydims =
            NULL; // No array dimensions set the pointer to null.
        curVarTypeInfo.typeString =
            strdup(variableTypeInfo.typeString);     // The type string.
        curVarTypeInfo.type = variableTypeInfo.type; // type enum.
        for (int i = 0; i < variableTypeInfo.numberofdims; i++) {
          // We want to get the ith expression of aparamlist and get the type
          // enum.
        }
      } else if (variableTypeInfo.numberofdims >
                 varDimsCount) { // Called with less array dimensions than
                                 // declared with in this case we are
                                 // returning an array type.
        curVarTypeInfo.numberofdims =
            variableTypeInfo.numberofdims - varDimsCount;
        curVarTypeInfo.arraydims =
            malloc(sizeof(int) * curVarTypeInfo.numberofdims);
        for (int i = 0; i < curVarTypeInfo.numberofdims; i++) {
          curVarTypeInfo.arraydims[i] = variableTypeInfo.arraydims[i];
        }
        curVarTypeInfo.type = variableTypeInfo.type;
        curVarTypeInfo.typeString = strdup(variableTypeInfo.typeString);
      } else {
        insert_error(arr,
                     create_error(get_name(astnode), err903, astnode->line));
        TypeInfo info1 = {NONE_TYPE, NULL, NULL, 0};
        return info1;
      }
      return curVarTypeInfo;
    }

    Scope *scopePtr = astnode->scope;

    while ((scopePtr = scopePtr->parentScope)->type != GLOBAL_SCOPE &&
           scopePtr != NULL) {

      entry = get_entry(scopePtr, name);

      if (entry->tableType == VARIABLE_ENTRY) {
        fprintf(stderr, "FOUND ENTRY %s IN CLASS SCOPE %s ... \n", name,
                scopePtr->scopeName);

        TypeInfo variableTypeInfo = entry->data.varEntry.type;
        TypeInfo curVarTypeInfo;
        if (variableTypeInfo.numberofdims == varDimsCount) {
          curVarTypeInfo.numberofdims = 0;
          curVarTypeInfo.arraydims = NULL;
          curVarTypeInfo.typeString = variableTypeInfo.typeString;
          curVarTypeInfo.type = variableTypeInfo.type;
        } else if (variableTypeInfo.numberofdims > varDimsCount) {
          curVarTypeInfo.numberofdims =
              variableTypeInfo.numberofdims - varDimsCount;
          curVarTypeInfo.arraydims =
              malloc(sizeof(int) * curVarTypeInfo.numberofdims);
          for (int i = 0; i < curVarTypeInfo.numberofdims; i++) {
            curVarTypeInfo.arraydims[i] = variableTypeInfo.arraydims[i];
          }
        } else {
          insert_error(arr,
                       create_error(get_name(astnode), err903, astnode->line));
          TypeInfo info1 = {NONE_TYPE, NULL, NULL, 0};
          return info1;
        }
        return curVarTypeInfo;
      }

      if (scopePtr->type == CLASS_SCOPE) {

        TableEntry *classEntry =
            get_entry(scopePtr->parentScope, scopePtr->scopeName);

        fprintf(stderr, "Checking inherited classes LOOKING FOR TYPE!...\n");
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

        while (stack->size > 0) {
          Scope *current = pop_scope(stack);
          fprintf(stderr, "Checking inherited class %s for entry %s ... \n",
                  current->scopeName, name);
          entry = get_entry(current, name);

          if (entry->tableType == VARIABLE_ENTRY &&
              strcmp(entry->data.varEntry.name, name) == 0) {
            fprintf(stderr, "FOUND ENTRY %s in SCOPE %s .\n", name,
                    current->scopeName);
            return entry->data.varEntry.type;
          }

          classEntry = get_entry(current->parentScope, current->scopeName);
          if (classEntry->data.classEntry.inheritsCount > 0 &&
              classEntry->data.classEntry.inheritedScopes != NULL) {
            for (int i = 0; i < classEntry->data.classEntry.inheritsCount;
                 i++) {
              push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
            }
          }
        }
      }
    }
    TypeInfo info = {NONE_TYPE, "nil", NULL, 0};
    return info;
  } else { // We have a member access to the variable.
    //
    //
    fprintf(stderr, "CHECKING MEMBER ACCESS VARIABLE %s ON LINE %d ... \n",
            name, astnode->line);

    TypeInfo leftInfo;

    // Get the type information of the class being accessed from.
    if (astnode->parent->children[0] ==
        astnode) // In this case we have a class access and we are the right
                 // child(bottom).
      leftInfo =
          get_type_expression(astnode->parent->children[1], arr, globalScope);
    else
      leftInfo = get_type_expression(astnode->parent->parent->children[1], arr,
                                     globalScope);

    Scope *scopePtr = astnode->scope;

    // Get the global scope.
    while (scopePtr->type != GLOBAL_SCOPE)
      scopePtr = scopePtr->parentScope;

    TableEntry *classEntry = get_entry(scopePtr, leftInfo.typeString);

    TableEntry *entry = get_entry(classEntry->data.classEntry.scope, name);

    // we found the entry
    if (entry->tableType == VARIABLE_ENTRY) {
      TypeInfo curVarInfo;
      TypeInfo tableEntryInfo;
      if (entry->tableType == VARIABLE_ENTRY) {
        tableEntryInfo = entry->data.varEntry.type;
        curVarInfo.type = tableEntryInfo.type;
        curVarInfo.typeString = tableEntryInfo.typeString;

      } else {
        tableEntryInfo = entry->data.fparamEntry.type;
        curVarInfo.type = tableEntryInfo.type;
        curVarInfo.typeString = tableEntryInfo.typeString;
      }
      if (varDimsCount <= tableEntryInfo.numberofdims) {
        curVarInfo.numberofdims = tableEntryInfo.numberofdims - varDimsCount;

        for (int i = 0; i < varDimsCount; i++) {
          TypeInfo indexInfo =
              get_type_expression(dimlistNode->children[i], arr, globalScope);
          if (indexInfo.type != INT_TYPE || indexInfo.numberofdims > 0 ||
              indexInfo.arraydims != NULL) {
            insert_error(arr, create_error(name, err2100, astnode->line));
            TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
            return temp;
          }
        }
      } else {
        insert_error(arr, create_error(name, err903, astnode->line));
        TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};

        return temp;
      }

      return curVarInfo;
    }

    if (classEntry->data.classEntry.inheritsCount > 0 &&
        classEntry->data.classEntry.inheritedScopes) {
      ScopeStack *stack = init_scope_stack();

      for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++) {
        push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
      }

      while (stack->size > 0) {
        Scope *current_class_scope = pop_scope(stack);
        TableEntry *entry = get_entry(current_class_scope, name);

        if (entry->tableType == VARIABLE_ENTRY) {
          TypeInfo curVarInfo;
          TypeInfo tableEntryInfo;
          if (entry->tableType == VARIABLE_ENTRY) {
            tableEntryInfo = entry->data.varEntry.type;
            curVarInfo.type = tableEntryInfo.type;
            curVarInfo.typeString = tableEntryInfo.typeString;

          } else {
            tableEntryInfo = entry->data.fparamEntry.type;
            curVarInfo.type = tableEntryInfo.type;
            curVarInfo.typeString = tableEntryInfo.typeString;
          }
          if (varDimsCount <= tableEntryInfo.numberofdims) {
            curVarInfo.numberofdims =
                tableEntryInfo.numberofdims - varDimsCount;

            for (int i = 0; i < varDimsCount; i++) {
              TypeInfo indexInfo = get_type_expression(dimlistNode->children[i],
                                                       arr, globalScope);
              if (indexInfo.type != INT_TYPE || indexInfo.numberofdims > 0 ||
                  indexInfo.arraydims != NULL) {
                insert_error(arr, create_error(name, err2100, astnode->line));
                TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
                return temp;
              }
            }
          } else {
            insert_error(arr, create_error(name, err903, astnode->line));
            TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};

            return temp;
          }

          return curVarInfo;
        } else { // We have to push the ihnerited scopes onto the stack in
                 // this case.
          TableEntry *classEntry = get_entry(current_class_scope->parentScope,
                                             current_class_scope->scopeName);
          if (classEntry->data.classEntry.inheritsCount > 0 &&
              classEntry->data.classEntry.inheritedScopes) {
            for (int i = 0; i < classEntry->data.classEntry.inheritsCount;
                 i++) {
              push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
            }
          }
        }
      }
    }
    TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
    return temp;
  }
} // End of get_type_var() function.

/*
 * Refactor of the get_type_functioncall() functioncall
 *
 * This function will return the type of a function call in the source code.
 *
 * We consider when we are accessing a class method vs when we are accessing a
 * free function. (check class scope first, check the global scope).
 */

TypeInfo get_type_functioncall2(node *astnode, Scope *globalScope,
                                ErrorArray *errors) {

  if (astnode->parent->type != dot ||
      (astnode->parent->type == funccall &&
       astnode->parent->parent->type == dot &&
       astnode->parent->children[1] == astnode)) {

    Scope *scopePtr = globalScope;
    while (scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
    }

    TableEntry *funcEntry = get_entry(scopePtr, get_name(astnode));

    if (funcEntry->tableType != FUNCDEF_ENTRY) {
      insert_error(errors,
                   create_error(get_name(astnode), err202, astnode->line));
      TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
      return temp;
    }

    return funcEntry->data.funcEntry.returnType;

  } else {
    TypeInfo classInfo;

    Scope *classScope;
    Scope *scopePtr = globalScope;

    while (scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
    }

    if (astnode == astnode->parent->children[0]) {
      classInfo = get_type_expression(astnode->parent->children[1], errors,
                                      globalScope);
      if (classInfo.type != ID_TYPE) {
        return classInfo;
      }
    } else {
      classInfo = get_type_expression(astnode->parent->parent->children[1],
                                      errors, globalScope);

      if (classInfo.type != ID_TYPE) {
        return classInfo;
      }
    }

    TableEntry *classEntry = get_entry(scopePtr, classInfo.typeString);

    if (classEntry->tableType != CLASS_ENTRY) {
      TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
      return temp;
    }
    classScope = classEntry->data.classEntry.scope;

    TableEntry *memberEntry = get_entry(classScope, get_name(astnode));

    if (memberEntry->tableType == FUNCDEF_ENTRY) {
      return memberEntry->data.funcEntry.returnType;
    }

    if (classEntry->data.classEntry.inheritsCount > 0 &&
        classEntry->data.classEntry.inheritedScopes) {
      ScopeStack *stack = init_scope_stack();

      for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++) {
        push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
      }

      while (stack->size > 0) {
        Scope *current_class_scope = pop_scope(stack);

        TableEntry *current_entry =
            get_entry(current_class_scope, get_name(astnode));

        if (current_entry->tableType == FUNCDEF_ENTRY) {
          return current_entry->data.funcEntry.returnType;
        }

        TableEntry *currentClassEntry = get_entry(
            current_class_scope->parentScope, current_class_scope->scopeName);

        if (currentClassEntry->tableType == CLASS_ENTRY &&
            currentClassEntry->data.classEntry.inheritsCount > 0 &&
            currentClassEntry->data.classEntry.inheritedScopes) {
          for (int i = 0; i < currentClassEntry->data.classEntry.inheritsCount;
               i++) {
            push_scope(currentClassEntry->data.classEntry.inheritedScopes[i],
                       stack);
          }
        }
      }
    }

    // We did not find the function in any scope that was searched.
    TypeInfo temp = {NONE_TYPE, "nil", NULL, 0};
    return temp;
  }
}

/*
 * get_type_functioncall() this will get the type of a function call.
 */
TypeInfo get_type_functioncall(node *astnode, Scope *globalScope) {

  const char *name = get_name(astnode);
  TableEntry *entry = get_entry(globalScope, name);

  if (entry->tableType == FUNCDEF_ENTRY &&
      entry->data.funcEntry.defined == true) {
    return entry->data.funcEntry.returnType;
  }

  for (int i = 0; i < SIZE; i++) {
    if (globalScope->entries[i].tableType == CLASS_ENTRY) {

      Scope *classScope = globalScope->entries[i].data.classEntry.scope;

      fprintf(stderr, "Checking class %s for function def %s ...\n",
              classScope->scopeName, name);
      TableEntry *funcDefEntry = get_entry(classScope, name);
      if (funcDefEntry->tableType == FUNCDEF_ENTRY) {
        return funcDefEntry->data.funcEntry.returnType;
      }
    }
  }

  // Not found.
  TypeInfo info = {NONE_TYPE, "nil", NULL, 0};
  return info;
}

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
    return get_type_var(astnode, arr, globalScope);
  } else if (astnode->type == funccall)
    return get_type_functioncall2(astnode, globalScope, arr);

  else if (astnode->type == dot) {

    node *left = astnode->children[1];
    node *right = astnode->children[0];

    if (right->type != dot) {

      TypeInfo leftInfo = get_type_expression(left, arr, globalScope);
      TableEntry *entry = get_entry(globalScope, leftInfo.typeString);

      EntryType expectedType;

      if (right->type == var)
        expectedType = VARIABLE_ENTRY;
      else
        expectedType = FUNCDEF_ENTRY;

      if (entry->tableType != CLASS_ENTRY) {
        insert_error(arr, create_error(get_name(left), err701, left->line));

        TypeInfo info = {NONE_TYPE, NULL, NULL, 0};
        return info;
      }

      Scope *scopePtr = entry->data.classEntry.scope;
      entry = get_entry(scopePtr, get_name(right));

      if (entry->tableType != expectedType) {
        insert_error(arr, create_error(get_name(right), err702, right->line));
        TypeInfo info = {NONE_TYPE, NULL, NULL, 0};
        return info;
      }

      return get_type_expression(right, arr, globalScope);
    }
    // Want to go down the dot chain and check each, we take a copy of the
    // globaScope pointer.
    Scope *scopePtr = globalScope;

    while (right->type == dot) {

      // This should not work! This makes absolutely no sense at all!
      TypeInfo leftInfo = get_type_expression(left, arr, scopePtr);
      TableEntry *entry = get_entry(
          globalScope, leftInfo.typeString); // We get the corresponding class
                                             // entry, if it exisits.

      // Left is not class type, cannot
      if (entry->tableType != CLASS_ENTRY) {

        insert_error(arr, create_error(get_name(left), err701, left->line));

        TypeInfo info = {NONE_TYPE, NULL, NULL, 0};
        return info;
      }

      scopePtr = entry->data.classEntry.scope;
      fprintf(stderr, "Entering scope %s looking if they have member %s ..\n",
              scopePtr->scopeName, get_name(right->children[1]));

      TableEntry *tmp = get_entry(scopePtr, get_name(right->children[1]));

      EntryType expectedType;
      if (right->children[1]->type == var)
        expectedType = VARIABLE_ENTRY;
      else
        expectedType = FUNCDEF_ENTRY;

      if (tmp->tableType != expectedType) {
        insert_error(arr, create_error(get_name(right->children[1]), err702,
                                       right->children[1]->line));
        TypeInfo info = {NONE_TYPE, NULL, NULL, 0};
        return info;
      }
      left = right->children[1];
      right = right->children[0];
    }

    return get_type_expression(right, arr, scopePtr);

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
 */
void validate_functioncall(node *astnode, Scope *globalScope, ErrorArray *arr) {

  // This function will check they type and the number of parameters a
  // function gets called with.

  if (astnode->parent->type != dot ||
      (astnode->parent->type == dot && astnode->parent->parent->type != dot &&
       astnode->parent->children[1] == astnode)) {

    TableEntry *functionEntry = get_entry(globalScope, get_name(astnode));

    if (functionEntry->tableType != FUNCDEF_ENTRY) {

      insert_error(arr,
                   create_error(get_name(astnode), err1401, astnode->line));
      return;
    }

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
        fprintf(stderr, "The type of the function call args: ");
        fprintf(stderr, "\n");

        if (!compare_type_info(fparamInfo, argInfo)) {
          insert_error(arr,
                       create_error(get_name(astnode), err1403, astnode->line));
        }
      }
    }
  } else {

    Scope *scopePtr = globalScope;

    while (scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
    }

    TypeInfo classTypeInfo;

    if (astnode->parent->children[0] == astnode) {
      classTypeInfo =
          get_type_expression(astnode->parent->children[1], arr, globalScope);
    } else {
      classTypeInfo = get_type_expression(astnode->parent->parent->children[1],
                                          arr, globalScope);
    }

    TableEntry *classEntry = get_entry(globalScope, classTypeInfo.typeString);

    /*
     * We need to refactor this as well what we should do is try to return the
     * error after we have exhausted the search.
     */
    if (classEntry->tableType == CLASS_ENTRY) {

      Scope *classScope = classEntry->data.classEntry.scope;
      TableEntry *functionEntry = get_entry(classScope, get_name(astnode));

      if (functionEntry->tableType == FUNCDEF_ENTRY) {
        // Now we must verify that we called with correct number and type of
        // parameters.

        int funccallParamsNum = get_aparams_count(astnode);
        int funcdefParamsNum = functionEntry->data.funcEntry.numfparams;

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
            TypeInfo fparamInfo = functionEntry->data.funcEntry.fparamslist[i]
                                      ->data.fparamEntry.type;
            node *currentDim = dimlistNode->children[i];
            TypeInfo argInfo =
                get_type_expression(currentDim, arr, globalScope);

            if (!compare_type_info(fparamInfo, argInfo)) {
              insert_error(
                  arr, create_error(get_name(astnode), err1403, astnode->line));
            }
          }
        }
        return;
      }

      if (classEntry->tableType == CLASS_ENTRY &&
          classEntry->data.classEntry.inheritsCount > 0 &&
          classEntry->data.classEntry.inheritedScopes) {
        ScopeStack *stack = init_scope_stack();

        for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++) {
          push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
        }

        while (stack->size > 0) {
          Scope *current_scope = pop_scope(stack);

          TableEntry *functionEntry =
              get_entry(current_scope, get_name(astnode));

          if (functionEntry->tableType == FUNCDEF_ENTRY) {
            int funccallParamsNum = get_aparams_count(astnode);
            int funcdefParamsNum = functionEntry->data.funcEntry.numfparams;

            if (funccallParamsNum != funcdefParamsNum) {

              insert_error(
                  arr, create_error(get_name(astnode), err1402, astnode->line));

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
                    functionEntry->data.funcEntry.fparamslist[i]
                        ->data.fparamEntry.type;
                node *currentDim = dimlistNode->children[i];
                TypeInfo argInfo =
                    get_type_expression(currentDim, arr, globalScope);

                if (!compare_type_info(fparamInfo, argInfo)) {
                  insert_error(arr, create_error(get_name(astnode), err1403,
                                                 astnode->line));
                }
              }
            }
          }

          classEntry =
              get_entry(current_scope->parentScope, current_scope->scopeName);
          if (classEntry->tableType == CLASS_ENTRY &&
              classEntry->data.classEntry.inheritsCount > 0 &&
              classEntry->data.classEntry.inheritedScopes) {

            for (int i = 0; i < classEntry->data.classEntry.inheritsCount;
                 i++) {
              push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
            }
          }
        }
      }

    } else { // The dot we are accessing from is not a class type! error
      insert_error(
          arr, create_error(classTypeInfo.typeString, err701, astnode->line));
    }
  }
}

/*
 * This function will be used during creation of the symbol table the
 * function will check if a variable being used has been declaerd.
 *
 * We must take into account wheter we are accessing from a member object or
 * freely from the scope.
 *
 * This function is not wokring correctly for whatever reason at the moment.
 */
int validate_lookup(node *astnode, ErrorArray *errors, Scope *globalScope) {

  const char *name = get_name(astnode);

  /*
   * For a free access of a variable within a scope.
   */
  if (astnode->parent->type != dot ||
      (astnode->parent->type == dot && astnode->parent->parent->type != dot &&
       astnode->parent->children[1] == astnode)) {

    Scope *scopePtr = astnode->scope;

    while (scopePtr->type != GLOBAL_SCOPE) {
      TableEntry *entry = get_entry(scopePtr, name);

      if (entry->tableType == VARIABLE_ENTRY ||
          (entry->tableType == FPARAM_ENTRY &&
           scopePtr->type == FUNCTION_SCOPE)) {
        return ok;
      }

      if (scopePtr->type == CLASS_SCOPE) {

        TableEntry *classEntry =
            get_entry(scopePtr->parentScope, scopePtr->scopeName);

        if (classEntry->tableType == CLASS_ENTRY &&
            classEntry->data.classEntry.inheritsCount > 0 &&
            classEntry->data.classEntry.inheritedScopes) {
          ScopeStack *stack = init_scope_stack();

          for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++)
            push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);

          while (stack->size > 0) {
            Scope *currentScope = pop_scope(stack);

            TableEntry *entry = get_entry(currentScope, name);

            if (entry->tableType == VARIABLE_ENTRY)
              return ok;

            classEntry =
                get_entry(currentScope->parentScope, currentScope->scopeName);

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
      scopePtr = scopePtr->parentScope;
    }
    return err111;
  } else { // For a member variable access.

    Scope *scopePtr = globalScope;

    while (scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
    }

    node *classVarNode = NULL;

    if (astnode == astnode->parent->children[0]) {
      classVarNode = astnode->parent->children[1];
    } else {
      classVarNode = astnode->parent->parent->children[1];
    }

    TypeInfo classInfo = get_type_expression(classVarNode, errors, globalScope);

    const char *name = get_name(astnode);
    TableEntry *classEntry = get_entry(scopePtr, classInfo.typeString);

    if (classEntry->tableType == CLASS_ENTRY) {
      TableEntry *entry = get_entry(classEntry->data.classEntry.scope, name);

      if (entry->tableType == VARIABLE_ENTRY) {
        return ok;
      }
    }

    if (classEntry->tableType == CLASS_ENTRY &&
        classEntry->data.classEntry.inheritsCount > 0 &&
        classEntry->data.classEntry.inheritedScopes) {
      ScopeStack *stack = init_scope_stack();

      for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++) {
        push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
      }

      while (stack->size > 0) {
        Scope *current_scope = pop_scope(stack);

        TableEntry *entry = get_entry(current_scope, name);

        if (entry->tableType == VARIABLE_ENTRY) {
          return ok;
        }

        classEntry =
            get_entry(current_scope->parentScope, current_scope->scopeName);

        if (classEntry->tableType == CLASS_ENTRY &&
            classEntry->data.classEntry.inheritsCount > 0 &&
            classEntry->data.classEntry.inheritedScopes) {
          for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++) {
            push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);
          }
        }
      }
    }
    return err111;
  }
}

/*
 * This function will be used to check if an assignment is valid, both sides
 * must be of the same type.
 */
FILE *temp;
void second_pass_type_check(node *root, Scope *globalScope,
                            ErrorArray *errors) {

  semantic_stack *stack = init_stack();

  check_for_shadowing_in_member_functions(globalScope, errors);
  check_tables_for_shadowing(globalScope, errors);

  push_node(root, stack);

  temp = fopen("no-scopes.txt", "w+");
  while (stack->size > 0) {
    node *current = pop_node(stack);

    if (current == NULL) {
      continue;
    }
    if (current->type == dot) {

      fprintf(stderr, "dot %d ", current->line);
    }
    if (current->type == funcdef) {
      check_for_return_statement_function(current, globalScope, errors);
    }
    if (current->type == funccall) {
      fprintf(stderr, "functioncall %s  %d ", get_name(current), current->line);
    }
    if (current->type == floatnum) {
      fprintf(stderr, "floatnum: ");
    }
    if (current->type == intnum) {
    }
    if (current->type == var) {
      fprintf(stderr, "var: ");
    }

    if (current->type == fparam && get_type_enum(current) == ID_TYPE) {

      const char *typeName = get_type_string(current);
      EntryType classType = get_entry(globalScope, typeName)->tableType;

      if (classType != CLASS_ENTRY) {
        fprintf(stderr, "Class has not been defined cannot declare a "
                        "function parameter "
                        "of this type.\n");
        insert_error(errors,
                     create_error(get_name(current), err206, current->line));

      } else {
        fprintf(stderr, "CLASS FOUND PUSHING TO STACK.\n");
        for (int i = 0; i < current->numchildren; i++)
          push_node(current->children[i], stack);
      }
    }

    if (current->type == funcdef && get_type_enum(current) == ID_TYPE) {

      const char *typeName = get_type_string(current);
      EntryType classType = get_entry(globalScope, typeName)->tableType;

      if (classType != CLASS_ENTRY) {
        fprintf(stderr, "The return type of the function %s.\n", typeName);
        fprintf(stderr, "Class has not been defined cannot declare a function "
                        "to have a return type "
                        "of this type.\n");
        insert_error(errors,
                     create_error(get_name(current), err207, current->line));
        continue;
      }
      Scope *containingScope = current->scope;
      TableEntry *funcEntry;
      if (containingScope->type == GLOBAL_SCOPE) {
        funcEntry = get_entry(containingScope, get_name(current));
        if (funcEntry->tableType == FUNCDEF_ENTRY) {
          for (int i = 0; i < current->numchildren; i++) {
            push_node(current->children[i], stack);
          }
        }
      }

      if (containingScope->type == CLASS_SCOPE) {
        funcEntry = get_entry(containingScope, get_name(current));
        if (funcEntry->tableType == FUNCDEF_ENTRY) {
          for (int i = 0; i < current->numchildren; i++) {
            push_node(current->children[i], stack);
          }
        }
      }

    } else if (current->type == funcdecl) {

      // We check if the entry has been defined.
      TableEntry *funcEntry = get_entry(current->scope, get_name(current));

      if (funcEntry->tableType == FUNCDEF_ENTRY &&
          funcEntry->data.funcEntry.defined == false) {
        insert_error(errors,
                     create_error(get_name(current), err900, current->line));
      }

    } else if (current->type == assingop) {

      TypeInfo LHSInfo =
          get_type_expression(current->children[0], errors, globalScope);
      TypeInfo RHSInfo =
          get_type_expression(current->children[1], errors, globalScope);

      if (!compare_type_info(LHSInfo, RHSInfo)) {

        fprintf(stderr, "LHS:");
        fprintf(stderr, "RHS:");

        insert_error(errors,
                     create_error(LHSInfo.typeString, err901, current->line));
      }

    } else if (current->type == multop || current->type == addop ||
               current->type == relexpr) {
      get_type_expression(current, errors, globalScope);

    } else if (current->type == returnnode) {

      node *returnValue = current->children[0];
      if (returnValue->scope ==
          NULL) // This will happen when we have a function that we
                // implemented but have no corresponding declaration.
        fprintf(stderr, "SCOPE IS NULL for RETURN ON LINE %d \n",
                current->line);
      else {
        // This is the type that we may receive.
        TypeInfo returnTypeExpected = get_entry(returnValue->scope->parentScope,
                                                returnValue->scope->scopeName)
                                          ->data.funcEntry.returnType;
        if (returnTypeExpected.type == VOID_TYPE) {
          insert_error(errors, create_error(returnValue->scope->scopeName,
                                            err1101, current->line));
        }

        // This is the type we actually get.
        TypeInfo typeReturned =
            get_type_expression(current->children[0], errors, globalScope);

        if (!compare_type_info(typeReturned, returnTypeExpected)) {
          insert_error(errors, create_error(returnTypeExpected.typeString,
                                            err1102, current->line));
        }
      }
    } else if (current->type == funccall) {
      validate_functioncall(current, globalScope, errors);
    } else {

      for (int i = 0; i < current->numchildren; i++) {
        if (current->children[i] != NULL)
          push_node(current->children[i], stack);
      }
    }
  }
}

/*
 * Note that some of the nodes are NOT given scopes, these include identifiers,
 * type, arraydims, etc these nodes just aggregate data they never get pushed
 * onto the stack! But we still need to push the children of these nodes onto
 * the stack!
 */
