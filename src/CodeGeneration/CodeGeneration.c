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

FILE *debugging_info = NULL;
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

/*
 *  ---NOTE----
 *  We need very large numbers for these values or else we encounter COLLISIONS
 * IN THE SYMBOL TABLE!!!!! We get problem that with long programs, we begin to
 * loop forever becuase we keep loading the first word in the stack frame, which
 * is the addrss of the first instruction of the subroutine.
 */
unsigned int temp_variable_number = 1000000;
unsigned int literal_entry_number = 1000000;
unsigned int if_numbers = 1000000;
unsigned int while_numbers = 1000000;

// This pointer will be used to access data that needs to be stored inside of
// the indexstorage.
int index_store_pointer = 0;
int index_load_pointer = 0;

void reset_index_pointers() {
  index_load_pointer = 0;
  index_store_pointer = 0;
}

bool free_registers[] = {true, true, true, true, true, true,
                         true, true, true, true, true, true};

void print_free_registers() {
  for (int i = 0; i < 12; i++) {
    if (free_registers[i])
      fprintf(stderr, "r%d, ", (i + 1));
  }
  fprintf(stderr, "\n");
}

void free_all_registers() {
  for (int i = 0; i < 12; i++)
    free_registers[i] = true;
}

/*
 * Helper function gets a number from a string.
 */
int extract_number_from_string(const char *str) {
  if (str == NULL) {
    return -1; // Return -1 or appropriate error code if input is NULL
  }

  // Assuming the string is always formatted correctly with 'r' followed by
  // numbers
  int num;
  char c;

  // Check if the first character is 'r' and the following is a number
  if (sscanf(str, "%c%d", &c, &num) == 2 && c == 'r') {
    return num;
  } else {
    return -1; // Return -1 or appropriate error code if parsing fails
  }
}

/*
 * This function is called to free a register once it is used.
 */
void free_register(const char *register_name) {
  int register_number = extract_number_from_string(register_name);
  register_number -= 1;

  free_registers[register_number] = true;
  fprintf(debugging_info, "FREEING REGISTER %d \n", register_number + 1);
  free((void *)register_name);
}
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
 * this function will just return a string for the label of an if
 */
char *get_next_if_label() {
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%s%d", "endif", if_numbers);

  return strdup(buffer);
}

/*
 * This function will get the else label
 */
char *get_next_else_label() {
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%s%d", "else",
           if_numbers++); // Note that we only increment it at the else block.
  return strdup(buffer);
}

/*
 * This function is used to get the offset for class members.
 *
 * What should we do if we want to access the members of a class.
 *
 * What if the variable that we are accessing is an array, what if the variable
 * that we are trying to access is a float, should we do the computations in the
 * registers in this case? Like we do with indexes?
 */
int get_class_member_offset(node *astnode, Scope *globalScope,
                            ErrorArray *errors, FILE *out) {
  // With this function we will get offset of a class member access this is
  // meant to be called when have a class member eg, a dot.

  // The base variable offset will always be the left child.
  node *left = astnode->children[1];
  node *right = astnode->children[0];

  Scope *scopePtr = globalScope;

  int base_varialbe_offset = get_offset(left, left->scope, errors, out);

  while (scopePtr && scopePtr->type != GLOBAL_SCOPE)
    scopePtr = scopePtr->parentScope;

  if (right->type != dot) {
    TypeInfo class_type_info = get_type_expression(left, errors, scopePtr);

    if (class_type_info.type != ID_TYPE) {
      fprintf(debugging_info,
              "ERROR TRYING TO USE CLASS MEMBER ACCESS ON NON CLASS TYPE!\n");
      return -1; // Error! we are trying to access a member from a non class
                 // type.
    }
    fprintf(debugging_info, "checking if %s is a member of variable %s\n",
            get_name((right)), get_name(left));

    TableEntry *class_entry = get_entry(scopePtr, class_type_info.typeString);
    TableEntry *member_entry =
        get_entry(class_entry->data.classEntry.scope, get_name(right));
    base_varialbe_offset += member_entry->offset;
    return base_varialbe_offset;
  } else {
    while (right->type == dot) {
      TypeInfo class_type_info = get_type_expression(left, errors, scopePtr);
      TableEntry *class_entry = get_entry(scopePtr, class_type_info.typeString);
      node *member_node = right->children[1];
      TableEntry *member_entry =
          get_entry(class_entry->data.classEntry.scope, get_name(member_node));
      base_varialbe_offset += member_entry->offset;

      left = right->children[1];
      right = right->children[0];
    }

    TypeInfo class_type_info = get_type_expression(left, errors, left->scope);
    TableEntry *class_entry = get_entry(scopePtr, class_type_info.typeString);
    TableEntry *member_entry =
        get_entry(class_entry->data.classEntry.scope, get_name(right));
    base_varialbe_offset += member_entry->offset;
    return base_varialbe_offset;
  }
}

/*
 * This function will deal with an if statement, we want to DFS through the file
 * and basically do the same things that the code_gen_pass function would do and
 * at the end, we put labels infront of each of the
 *
 * How should we do this what is causing this problem?
 * The problem occurs when we have neste
 */
void handle_if_statment(node *astnode, Scope *globalScope, ErrorArray *errors,
                        FILE *out) {

  node *expressionNode = astnode->children[2];
  node *thenNode = astnode->children[1];
  node *elseNode = astnode->children[0];

  int expressionOffset =
      handle_expression(expressionNode, errors, out, globalScope);

  char *comp_regiseter = get_next_free_register();
  if (expressionOffset)
    fprintf(out, "lw %s,%d(r14)\n", comp_regiseter, expressionOffset);
  else {
    const char *temp = get_next_free_register();
    fprintf(out, "addi %s,r0,%d\n", temp, index_load_pointer);
    fprintf(out, "lw %s, indexstorage(%s)\n", temp, temp);
    fprintf(out, "add %s,r14,%s\n", temp, temp);
    fprintf(out, "lw %s,0(%s)\n", comp_regiseter, temp);
  }

  const char *if_label = get_next_if_label();
  const char *else_label = get_next_else_label();

  fprintf(out, "bz %s,%s\n", comp_regiseter, else_label);
  free_register(comp_regiseter);

  code_gen_pass(thenNode, globalScope, out, errors);
  fprintf(out, "j %s\n", if_label);

  fprintf(out, "%s\n", else_label);
  code_gen_pass(elseNode, globalScope, out, errors);

  fprintf(out, "%s\n",
          if_label); // The problem is somewhere else, something is printing the
                     // same thing afer we have passed?
  //
}

/*
 * this functio will get the next while label.
 */
char *get_while_label() {
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%s%d", "while", while_numbers);
  return strdup(buffer);
}

/*
 * This function returns a label to the end of a while loop used for creating
 * the instrucions for a while loop.
 */
char *get_end_while_label() {
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%s%d", "endwhile", while_numbers++);
  return strdup(buffer);
}

/*This function will be used to handle while nodes in the code.
 */

void handle_while_statement(node *astnode, Scope *globalScope,
                            ErrorArray *errors, FILE *out) {

  node *expressionNode = astnode->children[1];
  node *bodyNode = astnode->children[0];

  char *whilelabele = get_while_label();
  char *enwhilelabel = get_end_while_label();

  fprintf(out, "%s\n", whilelabele);
  int expressionOffset =
      handle_expression(expressionNode, errors, out, globalScope);

  fprintf(debugging_info,
          "the value produced by the expression for the offset %d \n",
          expressionOffset);
  char *comp_regiseter = get_next_free_register();
  fprintf(out, "lw %s, %d(r14)\n", comp_regiseter, expressionOffset);
  fprintf(out, "bz %s,%s\n", comp_regiseter, enwhilelabel);
  free_register(comp_regiseter);
  code_gen_pass(bodyNode, globalScope, out,
                errors); // Note that we call thefunction recursively here, we
                         // call from the within a code_gen_pass call.
  fprintf(out, "j %s\n", whilelabele);
  fprintf(out, "%s\n", enwhilelabel);
}

/*
 * This function will return a string that corresponds to the first free
 * register in the register pool.
 *
 * Note we do not use a register pool, we just use the next register and then
 * store any temporary values in the memory of the stack frame.
 *
 * Now whenever we call get_next_free_register we neeed to call this function to
 * free the register.
 */

char *get_next_free_register() {

  int register_selected = -1;

  for (int i = 0; i < 12; i++) {
    if (free_registers[i] == true) {
      free_registers[i] = false;

      register_selected = i + 1;

      fprintf(debugging_info, "SELECTED REGISTER %d NO LONGER FREE \n",
              register_selected);
      break;
    }
  }

  if (register_selected < 0) {
    fprintf(debugging_info, "BIG PROBLEM NO FREE REGISTERS!!!\n");
    return NULL; // NO FREE REGISTERS BIG PROBLEM!!!
  }

  if (register_selected < 9) {
    char buffer[3];
    snprintf(buffer, sizeof(buffer), "r%d", register_selected);
    return strdup(buffer);
  } else {
    char buffer[4];
    snprintf(buffer, sizeof(buffer), "r%d", register_selected);
    return strdup(buffer);
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
    fprintf(debugging_info,
            "ERROR - create_litval_entry():Cannot allocate memory for "
            "table entry\n ");
    return NULL;
  }

  if (!entry) {
    fprintf(debugging_info,
            "ERROR - create_temp_entry(): Cannot allocate memory for "
            "new table entry.\n");
    return NULL;
  }
  if (astnode->type != floatnum && astnode->type != intnum) {
    fprintf(debugging_info,
            "ERROR - create_temp_entry(): Passed invalid type of node "
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
int get_variable_offset(node *astnode, ErrorArray *errors, FILE *out) {
  if (!astnode) {
    fprintf(debugging_info,
            "ERROR - get_variable_offset(): Cannot get offset node is "
            "null, exiting\n");
    return -1;
  }

  const char *name = get_name(astnode);

  node *dimlistNode = astnode->children[0];
  int arrayDims = dimlistNode->numchildren;

  // Free variable lookup, we begin search in the current scope.
  if (astnode->type == fparam || astnode->parent->type != dot ||
      (astnode->parent->type == dot && astnode->parent->parent->type != dot &&
       astnode->parent->children[1] == astnode)) {

    Scope *scopePtr = astnode->scope;

    TableEntry *entry = get_entry(scopePtr, name);

    // If we find the entry we return the size of the entry?
    if (entry->tableType == VARIABLE_ENTRY ||
        entry->tableType == FPARAM_ENTRY) {
      int typeSize;

      if (entry->data.varEntry.type.type == INT_TYPE) {
        typeSize = 4;
      } else if (entry->data.varEntry.type.type == FLOAT_TYPE)
        typeSize = 8;
      else if (entry->data.varEntry.type.type == ID_TYPE) {
        typeSize =
            get_class_size(entry->data.varEntry.type.typeString, scopePtr);
      } else {
        typeSize = 0;
      }

      int variableOffset = entry->offset;
      if (arrayDims) {

        const char *accumulator = get_next_free_register();

        fprintf(out, "addi %s,r0,%d %%s accumulator register is %s \n",
                accumulator, variableOffset, accumulator);

        for (int i = dimlistNode->numchildren - 1; i >= 0; i--) {
          if (i > 0) {
            int size_of_columns_to_add =
                entry->data.varEntry.type.arraydims[i - 1] * typeSize;

            int indexOffset = handle_expression(dimlistNode->children[i],
                                                errors, out, scopePtr);

            const char *index_reg = get_next_free_register();
            const char *column_size_reg = get_next_free_register();
            if (indexOffset == 0) {
              const char *temp = get_next_free_register();
              fprintf(out, "addi %s,r0,%d \n", temp, index_load_pointer);

              index_store_pointer -= 4;
              if (index_store_pointer > 0)
                index_load_pointer = index_store_pointer - 4;

              fprintf(out, "lw %s, indexstorage(%s)\n", temp, temp);
              fprintf(out, "add %s,%s,r14\n", temp, temp);
              fprintf(out, "lw %s, 0(%s)\n", index_reg, temp);
              free_register(temp);
            } else {
              fprintf(out, "lw %s, %d(r14)\n", index_reg, indexOffset);
            }
            fprintf(out, "addi %s,r0,%d %%s problem here\n", column_size_reg,
                    size_of_columns_to_add);

            fprintf(out, "mul %s,%s,%s\n", index_reg, index_reg,
                    column_size_reg);
            fprintf(out, "add %s,%s,%s\n", accumulator, accumulator, index_reg);

            free_register(index_reg);
            free_register(column_size_reg);

          } else {
            // We need to add the immediate to the accumulator.

            int indexExpression = handle_expression(dimlistNode->children[i],
                                                    errors, out, scopePtr);
            // Note that the index itself can be an array so we have to check
            // for this as well.
            const char *index_reg = get_next_free_register();
            if (indexExpression == 0) {
              const char *temp = get_next_free_register();
              fprintf(out, "addi %s,r0,%d \n", temp, index_load_pointer);

              index_store_pointer -= 4;
              if (index_store_pointer > 0)
                index_load_pointer = index_store_pointer - 4;

              fprintf(out, "lw %s, indexstorage(%s)\n", temp, temp);
              fprintf(out, "add %s,%s,r14\n", temp, temp);
              fprintf(out, "lw %s, 0(%s)\n", index_reg, temp);
              free_register(temp);

            } else {
              fprintf(out, "lw %s,%d(r14)\n", index_reg, indexExpression);
            }
            fprintf(out, "muli %s,%s,%d\n", index_reg, index_reg, typeSize);
            fprintf(out, "add %s,%s,%s\n", accumulator, accumulator, index_reg);
            free_register(index_reg);
          }
        }

        const char *temp = get_next_free_register();
        fprintf(out, "addi %s, r0, %d \n", temp, index_store_pointer);
        fprintf(out, "sw indexstorage(%s), %s\n", temp, accumulator);

        index_store_pointer += 4;
        index_load_pointer = index_store_pointer - 4;

        free_register(temp);
        free_register(accumulator);
        fprintf(debugging_info,
                "VALUE OF THE LOADING POINTER %d VALUE OF THE STORING POINTER "
                "%d ... \n",
                index_load_pointer, index_store_pointer);

        return 0;

      } else {
        return entry->offset; // If not we just return the offset as we
                              // normally would.
      }
    }

    while (scopePtr != NULL && scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;

      entry = get_entry(scopePtr, name);
      if (entry->tableType == VARIABLE_ENTRY) {

        int typeSize;

        if (entry->data.varEntry.type.type == INT_TYPE) {
          typeSize = 4;
        } else if (entry->data.varEntry.type.type == FLOAT_TYPE)
          typeSize = 8;
        else if (entry->data.varEntry.type.type == ID_TYPE) {
          typeSize =
              get_class_size(entry->data.varEntry.type.typeString, scopePtr);
        } else {
          typeSize = 0;
        }

        int variableOffset = entry->offset;
        if (arrayDims) {

          const char *accumulator = get_next_free_register();

          fprintf(out, "addi %s,r0,%d %%s accumulator register is %s \n",
                  accumulator, variableOffset, accumulator);

          const char *type_size_register = get_next_free_register();

          fprintf(out, "addi %s,r0,%d %%s the type size register is %s \n",
                  type_size_register, typeSize, type_size_register);

          for (int i = dimlistNode->numchildren - 1; i >= 0; i--) {
            if (i > 0) {
              const char *index_reg = get_next_free_register();
              int size_of_columns_to_add =
                  entry->data.varEntry.type.arraydims[i - 1] * typeSize;

              const char *column_size_reg = get_next_free_register();
              int indexOffset = handle_expression(dimlistNode->children[i],
                                                  errors, out, scopePtr);

              fprintf(out, "lw %s, %d(r14)\n", index_reg, indexOffset);
              fprintf(out, "addi %s,r0,%d\n", column_size_reg,
                      size_of_columns_to_add);

              fprintf(out, "mul %s,%s,%s\n", index_reg, index_reg,
                      column_size_reg);
              fprintf(out, "add %s,%s,%s\n", accumulator, accumulator,
                      index_reg);
            } else {

              int indexExpression = handle_expression(dimlistNode->children[i],
                                                      errors, out, scopePtr);
              const char *index_reg = get_next_free_register();
              fprintf(out, "lw %s,%d(r14)\n", index_reg, indexExpression);
              fprintf(out, "mul %s,%s,%s\n", index_reg, index_reg,
                      type_size_register);
              fprintf(out, "add %s,%s,%s\n", accumulator, accumulator,
                      index_reg);
            }
          }

          const char *temp = get_next_free_register();
          fprintf(out, "addi %s, r0, %d %%s PRoblem here\n", temp,
                  index_store_pointer);
          fprintf(out, "sw indexstorage(%s), %s\n", temp, accumulator);
          index_store_pointer += 4;
          index_load_pointer =
              index_load_pointer -
              4; // We want a pointer to the last accessible byte.

          return 0; // We indicate to the calling function that we are returning
                    // the offset for an array type so we should look in the
                    // "indexstorage" section that we have reserved.
        } else {
          return entry->offset; // If not we just return the offset as we
                                // normally would.
        }
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

            if (entry->tableType == VARIABLE_ENTRY ||
                entry->tableType == FPARAM_ENTRY) {
              int typeSize;

              if (entry->data.varEntry.type.type == INT_TYPE) {
                typeSize = 4;
              } else if (entry->data.varEntry.type.type == FLOAT_TYPE)
                typeSize = 8;
              else if (entry->data.varEntry.type.type == ID_TYPE) {
                typeSize = get_class_size(entry->data.varEntry.type.typeString,
                                          scopePtr);
              } else {
                typeSize = 0;
              }

              int variableOffset = entry->offset;
              if (arrayDims) {

                const char *accumulator = get_next_free_register();

                fprintf(out, "addi %s,r0,%d %%s accumulator register is %s \n",
                        accumulator, variableOffset, accumulator);

                const char *type_size_register = get_next_free_register();

                fprintf(out,
                        "addi %s,r0,%d %%s the type size register is %s \n",
                        type_size_register, typeSize, type_size_register);

                for (int i = dimlistNode->numchildren - 1; i >= 0; i--) {
                  if (i > 0) {
                    const char *index_reg = get_next_free_register();
                    int size_of_columns_to_add =
                        entry->data.varEntry.type.arraydims[i - 1] * typeSize;

                    const char *column_size_reg = get_next_free_register();
                    int indexOffset = handle_expression(
                        dimlistNode->children[i], errors, out, scopePtr);

                    fprintf(out, "lw %s, %d(r14)\n", index_reg, indexOffset);
                    fprintf(out, "addi %s,r0,%d\n", column_size_reg,
                            size_of_columns_to_add);

                    fprintf(out, "mul %s,%s,%s\n", index_reg, index_reg,
                            column_size_reg);
                    fprintf(out, "add %s,%s,%s\n", accumulator, accumulator,
                            index_reg);
                  } else {
                    // We need to add the immediate to the accumulator.

                    int indexExpression = handle_expression(
                        dimlistNode->children[i], errors, out, scopePtr);
                    const char *index_reg = get_next_free_register();
                    fprintf(out, "lw %s,%d(r14)\n", index_reg, indexExpression);
                    fprintf(out, "mul %s,%s,%s\n", index_reg, index_reg,
                            type_size_register);
                    fprintf(out, "add %s,%s,%s\n", accumulator, accumulator,
                            index_reg);
                  }
                }

                const char *temp = get_next_free_register();
                fprintf(out, "addi %s, r0, %d\n", temp, index_store_pointer);
                fprintf(out, "sw indexstorage(%s), %s\n", temp, accumulator);
                index_store_pointer += 4;
                index_load_pointer =
                    index_load_pointer -
                    4;    // We want a pointer to the last accessible byte.
                          // predictable place in memory.
                return 0; // We indicate to the calling function that we are
                          // returning the offset for an array type so we should
                          // look in the "indexstorage" section that we have
                          // reserved.
              } else {
                return entry->offset; // If not we just return the offset as we
                                      // normally would.
              }
            }

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

      if (entry->tableType == VARIABLE_ENTRY) {
        int typeSize;

        if (entry->data.varEntry.type.type == INT_TYPE) {
          typeSize = 4;
        } else if (entry->data.varEntry.type.type == FLOAT_TYPE)
          typeSize = 8;
        else if (entry->data.varEntry.type.type == ID_TYPE) {
          typeSize =
              get_class_size(entry->data.varEntry.type.typeString, scopePtr);
        } else {
          typeSize = 0;
        }

        int variableOffset = entry->offset;
        if (arrayDims) {

          const char *accumulator = get_next_free_register();

          fprintf(out, "addi %s,r0,%d %%s accumulator register is %s \n",
                  accumulator, variableOffset, accumulator);

          const char *type_size_register = get_next_free_register();

          fprintf(out, "addi %s,r0,%d %%s the type size register is %s \n",
                  type_size_register, typeSize, type_size_register);

          for (int i = dimlistNode->numchildren - 1; i >= 0; i--) {
            if (i > 0) {
              const char *index_reg = get_next_free_register();
              int size_of_columns_to_add =
                  entry->data.varEntry.type.arraydims[i - 1] * typeSize;

              const char *column_size_reg = get_next_free_register();
              int indexOffset = handle_expression(dimlistNode->children[i],
                                                  errors, out, scopePtr);

              fprintf(out, "lw %s, %d(r14)\n", index_reg, indexOffset);
              fprintf(out, "addi %s,r0,%d\n", column_size_reg,
                      size_of_columns_to_add);

              fprintf(out, "mul %s,%s,%s\n", index_reg, index_reg,
                      column_size_reg);
              fprintf(out, "add %s,%s,%s\n", accumulator, accumulator,
                      index_reg);
            } else {
              // We need to add the immediate to the accumulator.

              int indexExpression = handle_expression(dimlistNode->children[i],
                                                      errors, out, scopePtr);
              const char *index_reg = get_next_free_register();
              fprintf(out, "lw %s,%d(r14)\n", index_reg, indexExpression);
              fprintf(out, "mul %s,%s,%s\n", index_reg, index_reg,
                      type_size_register);
              fprintf(out, "add %s,%s,%s\n", accumulator, accumulator,
                      index_reg);
            }
          }

          const char *temp = get_next_free_register();
          fprintf(out, "addi %s, r0, %d\n", temp, index_store_pointer);
          fprintf(out, "sw indexstorage(%s), %s\n", temp, accumulator);
          index_store_pointer += 4;
          index_load_pointer =
              index_load_pointer -
              4; // We want a pointer to the last accessible byte.

          return 0; // We indicate to the calling function that we are
                    // returning the offset for an array type so we should
                    // look in the "indexstorage" section that we have reserved.
        } else {
          return entry->offset; // If not we just return the offset as we
                                // normally would.
        }
      }

      if (classEntry->data.classEntry.inheritsCount > 0 &&
          classEntry->data.classEntry.inheritedScopes) {
        ScopeStack *stack = init_scope_stack();

        for (int i = 0; i < classEntry->data.classEntry.inheritsCount; i++)
          push_scope(classEntry->data.classEntry.inheritedScopes[i], stack);

        while (stack->size > 0) {
          Scope *current_scope = pop_scope(stack);

          entry = get_entry(current_scope, name);
          if (entry->tableType == VARIABLE_ENTRY) {
            int typeSize;

            if (entry->data.varEntry.type.type == INT_TYPE) {
              typeSize = 4;
            } else if (entry->data.varEntry.type.type == FLOAT_TYPE)
              typeSize = 8;
            else if (entry->data.varEntry.type.type == ID_TYPE) {
              typeSize = get_class_size(entry->data.varEntry.type.typeString,
                                        scopePtr);
            } else {
              typeSize = 0;
            }

            int variableOffset = entry->offset;
            if (arrayDims) {

              const char *accumulator = get_next_free_register();

              fprintf(out, "addi %s,r0,%d %%s accumulator register is %s \n",
                      accumulator, variableOffset, accumulator);

              const char *type_size_register = get_next_free_register();

              fprintf(out, "addi %s,r0,%d %%s the type size register is %s \n",
                      type_size_register, typeSize, type_size_register);

              for (int i = dimlistNode->numchildren - 1; i >= 0; i--) {
                if (i > 0) {
                  const char *index_reg = get_next_free_register();
                  int size_of_columns_to_add =
                      entry->data.varEntry.type.arraydims[i - 1] * typeSize;

                  const char *column_size_reg = get_next_free_register();
                  int indexOffset = handle_expression(dimlistNode->children[i],
                                                      errors, out, scopePtr);

                  fprintf(out, "lw %s, %d(r14)\n", index_reg, indexOffset);
                  fprintf(out, "addi %s,r0,%d\n", column_size_reg,
                          size_of_columns_to_add);

                  fprintf(out, "mul %s,%s,%s\n", index_reg, index_reg,
                          column_size_reg);
                  fprintf(out, "add %s,%s,%s\n", accumulator, accumulator,
                          index_reg);
                } else {
                  // We need to add the immediate to the accumulator.

                  int indexExpression = handle_expression(
                      dimlistNode->children[i], errors, out, scopePtr);
                  const char *index_reg = get_next_free_register();
                  fprintf(out, "lw %s,%d(r14)\n", index_reg, indexExpression);
                  fprintf(out, "mul %s,%s,%s\n", index_reg, index_reg,
                          type_size_register);
                  fprintf(out, "add %s,%s,%s\n", accumulator, accumulator,
                          index_reg);
                }
              }

              const char *temp = get_next_free_register();
              fprintf(out, "addi %s, r0, %d\n", temp, index_store_pointer);
              fprintf(out, "sw indexstorage(%s), %s\n", temp, accumulator);
              index_store_pointer += 4;
              index_load_pointer =
                  index_store_pointer -
                  4; // We want a pointer to the last accessible
                     // byte. predictable place in memory.

              return 0; // We indicate to the calling function that we are
                        // returning the offset for an array type so we should
                        // look in the "indexstorage" section that we have
                        // reserved.
            } else {
              return entry->offset; // If not we just return the offset as we
                                    // normally would.
            }
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
} // End of the get_variable_offset function.

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
        fprintf(debugging_info, "BIG PROBLEM!!!\n");
      }
      if (scopePtr->type == GLOBAL_SCOPE)
        fprintf(debugging_info, "FOUND GLOBAL SCOPE!\n");
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
    return get_variable_offset(astnode, errors, out);

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
  } else if (astnode->type == fparam) {
    return get_variable_offset(astnode, errors, out);
  }

  return 0;
}

/*
 * This function will return the offset to of a class member access.
 * Note that all that we have to do is get the offset from the class symbol
 * table then add it to the offset of the variable, if we have a function that
 * returns a class object that also has
 *
 * This will basically be getting the offset of a class member.
 *
 * Note that we are always going to be in the same stack frame.
 */

/*
int handle_dots(node *astnode, Scope *globalScope, ErrorArray *errors,
                FILE *out) {

  node *right = astnode->children[0];
  node *left = astnode->children[1];

  Scope *scopePtr = globalScope;
  while (scopePtr && scopePtr->type != GLOBAL_SCOPE) {
    scopePtr = scopePtr->parentScope;
  }
  // We need to get the offset of the left handside.
  if (right->type != dot) {
  }

  while (right->type == dot) {
    TypeInfo classInfo = get_type_expression(left, errors, left->scope);
  }
}
*/

/*
 *
 * How would we handle an expression that uses
 */
int handle_expression(node *astnode, ErrorArray *errors, FILE *out,
                      Scope *globalScope) {

  if (astnode->type == floatnum || astnode->type == intnum) {

    int offset = get_offset(astnode, astnode->scope, errors, out);
    const char *registerBeingUsed = get_next_free_register();

    fprintf(out, "addi %s,r0,%s \n", registerBeingUsed,
            astnode->value); // We store the value in a register.
    fprintf(out, "sw %d(r14),%s\n", offset,
            registerBeingUsed); // Store the value that is being stored in the
                                // register in the corresponding offset.
    free_register(registerBeingUsed);

    return offset;

  } else if (astnode->type == multop || astnode->type == addop) {

    node *left = astnode->children[1];
    node *right = astnode->children[0];

    // We get the offsets note that if either is zero we need to get the offsets
    // that are stored
    int rightOffset = handle_expression(right, errors, out, globalScope);
    int leftOffset = handle_expression(left, errors, out, globalScope);

    int operationOffset = get_offset(astnode, astnode->scope, errors, out);

    if (!operationOffset) {
      if (astnode->scope == NULL)
        fprintf(stderr, "ERROR SCOPE OF THE NODE IS NULL!!!\n");
      fprintf(stderr, "Cannot find %s in the current scope problem!!!\n",
              astnode->entryName);
      fprintf(stderr, "OPERATION OFFSET IS 0!! PROBLEM !!!\n");
    }
    const char *left_register = get_next_free_register();
    const char *right_register = get_next_free_register();
    const char *operation_register = get_next_free_register();

    char operationBuffer[16];

    if (strcmp(astnode->value, "+") == 0)
      snprintf(operationBuffer, sizeof(operationBuffer), "add");
    else if (!strcmp(astnode->value, "*"))
      snprintf(operationBuffer, sizeof(operationBuffer), "mul");
    else if (!strcmp(astnode->value, "-"))
      snprintf(operationBuffer, sizeof(operationBuffer), "sub");
    else if (!strcmp(astnode->value, "/"))
      snprintf(operationBuffer, sizeof(operationBuffer), "div");
    else if (!strcmp(astnode->value, "|"))
      snprintf(operationBuffer, sizeof(operationBuffer), "or");
    else if (!strcmp(astnode->value, "&"))
      snprintf(operationBuffer, sizeof(operationBuffer), "and");

    if (rightOffset) {
      fprintf(out, "lw %s,%d(r14)\n", right_register, rightOffset);
    } else {
      const char *temp = get_next_free_register();

      fprintf(debugging_info, "LOADING OFFSET FROM LOAD POINTER OF VALUE %d\n",
              index_load_pointer);
      fprintf(out, "addi %s,r0,%d\n", temp,
              index_load_pointer); // Get the value of the offset into the haep

      index_store_pointer -= 4;
      if (index_store_pointer == 0)
        index_load_pointer = 0;
      else
        index_load_pointer = index_store_pointer - 4;

      fprintf(out, "lw %s, indexstorage(%s) %%s HERE1!!\n", temp,
              temp); // Load the value of the offset into the indexstorage.
      fprintf(out, "add %s,r14,%s\n", temp,
              temp); // Add the stack frame pointer to this value.
      fprintf(out, "lw %s, 0(%s)\n", right_register,
              temp); // Load the value of the offset into the stack
                     // frame into the left operand register.
      free_register(temp);
    }
    if (leftOffset) {
      fprintf(out, "lw %s, %d(r14)\n", left_register, leftOffset);
    } else {

      fprintf(debugging_info, "LOADING OFFSET FROM LOAD POINTER OF VALUE %d\n",
              index_load_pointer);
      const char *temp = get_next_free_register();

      fprintf(out, "addi %s,r0,%d\n", temp,
              index_load_pointer); // Get the value of the offset into the haep

      index_store_pointer -= 4;
      if (index_store_pointer == 0)
        index_load_pointer = 0;
      else
        index_load_pointer = index_store_pointer - 4;
      fprintf(debugging_info, "IN THE LEFT OFFSET THE VALUE OF LOAD %d \n",
              index_load_pointer);

      fprintf(out, "lw %s, indexstorage(%s) %%s HERE2 !!!\n", temp,
              temp); // Load the value of the offset into the indexstorage.
      fprintf(out, "add %s,r14,%s\n", temp,
              temp); // Add the stack frame pointer to this value.
      fprintf(out, "lw %s, 0(%s)\n", left_register,
              temp); // Load the value of the offset into the stack
                     // frame into the left operand register.
      free_register(temp);
    }

    fprintf(
        out, "%s %s,%s,%s\n", operationBuffer,
        operation_register, // perform the operation and store it in a buffer.
        left_register, right_register);

    free_register(left_register);
    free_register(right_register);

    fprintf(
        out, "sw %d(r14),%s\n", operationOffset,
        operation_register); // Store the word in the operations given offset.

    free_register(operation_register);
    return operationOffset;
  } else if (astnode->type == var) {

    return get_offset(astnode, globalScope, errors, out);

  } else if (astnode->type == dot) {
    int x = get_class_member_offset(astnode, astnode->scope, errors, out);
    if (x == 0) {
      fprintf(debugging_info, "PROBLEM!!!\n");
      exit(0);
    }
    return x;

  } else if (astnode->type == funccall) {

    handle_function_call(astnode, errors, globalScope, out);

    int functionOffset = get_offset(astnode, globalScope, errors, out);

    // We store the return value here.
    fprintf(out, "sw %d(r14),r13\n", functionOffset);
    return functionOffset;
  } else if (astnode->type == sign) {

    int current_offset =
        handle_expression(astnode->children[0], errors, out, globalScope);

    const char *temp_register = get_next_free_register();

    // We load the word at the offset, multiply by negative one.
    fprintf(out, "lw %s,%d(r14)\n", temp_register, current_offset);
    fprintf(out, "muli %s,%s,-1\n", temp_register,
            temp_register); // Multiply the value by -1.
    fprintf(out, "sw %d(r14),%s\n", current_offset, temp_register);
    free_register(temp_register);

    return current_offset;

  } else if (astnode->type == notnode) {
    int current_offset =
        handle_expression(astnode->children[0], errors, out, globalScope);

    const char *temp_register = get_next_free_register();
    fprintf(out, "lw %s, %d(r14)\n", temp_register, current_offset);
    fprintf(out, "not %s,%s\n", temp_register, temp_register);
    fprintf(out, "sw %d(r14),%s\n", current_offset, temp_register);
    return current_offset;

  } else if (astnode->type == relexpr) {

    node *leftOperand = astnode->children[2];
    node *operationNode = astnode->children[1];
    node *rightNode = astnode->children[0];

    const char *operationValue = operationNode->value;

    int operationOffset = get_offset(astnode, globalScope, errors, out);
    int leftOffset = handle_expression(leftOperand, errors, out, globalScope);
    int rightOffset = handle_expression(rightNode, errors, out, globalScope);

    const char *instructionOperation = NULL;
    if (!strcmp(operationValue, "eq"))
      instructionOperation = "ceq";
    else if (!strcmp(operationValue, "neq"))
      instructionOperation = "cne";
    else if (!strcmp(operationValue, "lt"))
      instructionOperation = "clt";
    else if (!strcmp(operationValue, "gt"))
      instructionOperation = "cgt";
    else if (!strcmp(operationValue, "geq"))
      instructionOperation = "cge";
    else if (!strcmp(operationValue, "leq"))
      instructionOperation = "cle";

    const char *right_register = get_next_free_register();
    const char *left_register = get_next_free_register();
    const char *opertion_register = get_next_free_register();

    if (rightOffset) {

      fprintf(out, "lw %s, %d(r14)\n", right_register, rightOffset);
    } else {

      const char *temp = get_next_free_register();

      fprintf(debugging_info, "LOADING OFFSET FROM LOAD POINTER OF VALUE %d\n",
              index_load_pointer);
      fprintf(out, "addi %s,r0,%d\n", temp,
              index_load_pointer); // Get the value of the offset into the haep

      index_store_pointer -= 4;
      if (index_store_pointer == 0)
        index_load_pointer = 0;
      else
        index_load_pointer = index_store_pointer - 4;

      fprintf(out, "lw %s, indexstorage(%s) %%s HERE1!!\n", temp,
              temp); // Load the value of the offset into the indexstorage.
      fprintf(out, "add %s,r14,%s\n", temp,
              temp); // Add the stack frame pointer to this value.
      fprintf(out, "lw %s, 0(%s)\n", right_register,
              temp); // Load the value of the offset into the stack
                     // frame into the left operand register.
      free_register(temp);
    }

    if (leftOffset) {

      fprintf(out, "lw %s,%d(r14) %%s relexpr\n", left_register, leftOffset);
    } else {

      fprintf(debugging_info, "LOADING OFFSET FROM LOAD POINTER OF VALUE %d\n",
              index_load_pointer);
      const char *temp = get_next_free_register();

      fprintf(out, "addi %s,r0,%d\n", temp,
              index_load_pointer); // Get the value of the offset into the haep

      index_store_pointer -= 4;
      if (index_store_pointer == 0)
        index_load_pointer = 0;
      else
        index_load_pointer = index_store_pointer - 4;
      fprintf(debugging_info, "IN THE LEFT OFFSET THE VALUE OF LOAD %d \n",
              index_load_pointer);

      fprintf(out, "lw %s, indexstorage(%s) %%s HERE2 !!!\n", temp,
              temp); // Load the value of the offset into the indexstorage.
      fprintf(out, "add %s,r14,%s\n", temp,
              temp); // Add the stack frame pointer to this value.
      fprintf(out, "lw %s, 0(%s)\n", left_register,
              temp); // Load the value of the offset into the stack
                     // frame into the left operand register.
      free_register(temp);
    }

    fprintf(out, "%s %s,%s,%s\n", instructionOperation, opertion_register,
            left_register, right_register);
    fprintf(out, "sw %d(r14), %s\n", operationOffset, opertion_register);

    free_register(left_register);
    free_register(right_register);
    free_register(opertion_register);
    return operationOffset;
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
 *
 * We need to reorganize this part, now that we have the ability to retrn
 * arrays, an array being return is denoted with an offset of zero being
 * returned, this means that we must first look into the indexstorage and find
 * the data that we are looking for at indexstorage(r0).
 *
 * So what we need to do is the following.
 *
 */
void handle_assignment_statement(node *astnode, ErrorArray *errors,
                                 Scope *globalScope, FILE *out) {

  int value_offset =
      handle_expression(astnode->children[0], errors, out, globalScope);
  int assignment_offset =
      handle_expression(astnode->children[1], errors, out, globalScope);

  const char *value_register = get_next_free_register();
  if (value_offset) {
    fprintf(out, "lw %s,%d(r14)\n", value_register, value_offset);
  } else {
    // we need to get the offset from the indexstorage.
    const char *temp = get_next_free_register();

    fprintf(debugging_info, "LOADING OFFSET FROM LOAD POINTER OF VALUE %d\n",
            index_load_pointer);
    fprintf(out, "addi %s,r0,%d\n", temp, index_load_pointer);
    index_store_pointer -= 4;
    if (index_store_pointer > 0)
      index_load_pointer = index_store_pointer - 4;
    else
      index_load_pointer = 0;
    fprintf(out, "lw %s, indexstorage(%s) %%s here !!!\n", temp, temp);
    fprintf(out, "add %s,r14,%s\n", temp, temp);
    fprintf(out, "lw %s, 0(%s)\n", value_register, temp);
    free_register(temp);
  }

  // After this the value register has been loaded with the correct values.
  if (assignment_offset) {
    fprintf(out, "sw %d(r14),%s\n", assignment_offset, value_register);
    free_register(value_register);
  } else {

    fprintf(debugging_info, "LOADING OFFSET FROM LOAD POINTER OF VALUE %d\n",
            index_load_pointer);
    const char *temp = get_next_free_register();
    fprintf(out, "addi %s,r0,%d\n", temp, index_load_pointer);
    index_store_pointer -= 4;
    if (index_store_pointer > 0)
      index_load_pointer = index_store_pointer - 4;
    else
      index_load_pointer = 0;
    fprintf(out, "lw %s, indexstorage(%s) %%s HERE !!\n", temp, temp);
    fprintf(out, "add %s,r14,%s\n", temp, temp);
    fprintf(out, "sw 0(%s),%s\n", temp, value_register);
    free_register(value_register);
    free_register(temp);
  }
}

/*
 * This function gets function parameters list from a function call node.
 */
TableEntry *get_function_entry(Scope *currentScope, node *functionNode,
                               ErrorArray *errors) {
  Scope *scopePtr = currentScope;

  const char *name = get_name(functionNode);
  fprintf(debugging_info, "looking for the function %s ...\n", name);

  if (scopePtr && scopePtr->type != GLOBAL_SCOPE) {
    fprintf(debugging_info, "Global scope has been found\n");
    scopePtr = scopePtr->parentScope;
  }

  if (functionNode->parent->type != dot ||
      (functionNode->parent->type == dot &&
       functionNode->parent->parent->type != dot &&
       functionNode->parent->children[1] == functionNode)) {

    fprintf(debugging_info, "looking for free function %s in scope %s ...\n",
            name, scopePtr->scopeName);
    TableEntry *funcEntry = get_entry(scopePtr, name);

    if (funcEntry->tableType == FUNCDEF_ENTRY) {
      fprintf(debugging_info, "found function %s in scope %s, returning ... \n",
              name, scopePtr->scopeName);
      return funcEntry;
    } else {
      return NULL;
    }
  } else {

    while (scopePtr && scopePtr->type != GLOBAL_SCOPE)
      scopePtr = scopePtr->parentScope;

    TypeInfo class_type_info;

    if (functionNode->parent->children[0] == functionNode) {
      class_type_info = get_type_expression(functionNode->parent->children[1],
                                            errors, functionNode->scope);
    } else {
      class_type_info =
          get_type_expression(functionNode->parent->parent->children[1], errors,
                              functionNode->scope);
    }

    fprintf(debugging_info, "Name of class: %s name of member: %s\n",
            class_type_info.typeString, name);

    TableEntry *class_entry = get_entry(scopePtr, class_type_info.typeString);
    if (class_entry->tableType == CLASS_ENTRY) {
      fprintf(debugging_info, "The name of the class entry is %s ...\n",
              class_entry->data.classEntry.name);
      scopePtr = class_entry->data.classEntry.scope;
      TableEntry *func_entry = get_entry(scopePtr, name);
      if (func_entry->tableType == FUNCDEF_ENTRY) {
        fprintf(debugging_info, "Found function entry!\n");
        return func_entry;
      }

      if (class_entry->tableType == CLASS_ENTRY &&
          class_entry->data.classEntry.inheritsCount &&
          class_entry->data.classEntry.inheritedScopes) {
        ScopeStack *stack = init_scope_stack();

        for (int i = 0; i < class_entry->data.classEntry.inheritsCount; i++)
          push_scope(class_entry->data.classEntry.inheritedScopes[i], stack);

        while (stack->size > 0) {
          Scope *current = pop_scope(stack);

          func_entry = get_entry(current, name);
          if (func_entry->tableType == FUNCDEF_ENTRY)
            return func_entry;

          class_entry = get_entry(current->parentScope, current->scopeName);

          if (class_entry->tableType == CLASS_ENTRY &&
              class_entry->data.classEntry.inheritsCount &&
              class_entry->data.classEntry.inheritedScopes) {
            for (int i = 0; i < class_entry->data.classEntry.inheritsCount;
                 i++) {
              push_scope(class_entry->data.classEntry.inheritedScopes[i],
                         stack);
            }
          }
        }
      }
    }
    return NULL;
  }
}

TableEntry *get_function_definition_entry(node *astnode, Scope *global_scope,
                                          ErrorArray *errors, FILE *out) {

  const char *name = get_name(astnode);
  fprintf(debugging_info,
          "In get_function_definition_entry looking for function %s ... \n",
          name);
  Scope *scopePtr = global_scope;

  while (scopePtr && scopePtr->type != GLOBAL_SCOPE)
    scopePtr = scopePtr->parentScope;

  if (scopePtr->type == GLOBAL_SCOPE) {
    fprintf(debugging_info, "GLOBAL SCOPE FOUND !\n");
  } else {
    fprintf(debugging_info, "PROBLEM SCOPE IS NOW NULL!!!\n");
  }

  /*Method need to access the class.
   */
  if (astnode->parent->type == impldef) {
    fprintf(debugging_info, "looking for a member function %s ... \n", name);
    const char *class_name = get_name(astnode->parent);
    fprintf(debugging_info, "the name of the class is %s ... \n", class_name);
    TableEntry *class_entry = get_entry(scopePtr, class_name);
    if (class_entry->tableType == CLASS_ENTRY) {
      TableEntry *func_entry =
          get_entry(class_entry->data.classEntry.scope, name);
      if (func_entry->tableType == FUNCDEF_ENTRY)
        return func_entry;

      if (class_entry->tableType == CLASS_ENTRY &&
          class_entry->data.classEntry.inheritsCount &&
          class_entry->data.classEntry.inheritedScopes) {
        ScopeStack *stack = init_scope_stack();
        while (stack->size > 0) {
          Scope *current = pop_scope(stack);
          func_entry = get_entry(current, name);

          if (func_entry->tableType == FUNCDEF_ENTRY)
            return func_entry;

          class_entry = get_entry(current->parentScope, current->scopeName);

          if (class_entry && class_entry->tableType == CLASS_ENTRY &&
              class_entry->data.classEntry.inheritedScopes &&
              class_entry->data.classEntry.inheritsCount) {
            for (int i = 0; i < class_entry->data.classEntry.inheritsCount;
                 i++) {
              push_scope(class_entry->data.classEntry.inheritedScopes[i],
                         stack);
            }
          }
        }
      }
    }
    return NULL; // function definition entry could not be found.
  } else {
    // We only need to search in the global scope.

    TableEntry *func_entry = get_entry(scopePtr, name);
    if (func_entry->tableType == FUNCDEF_ENTRY)
      return func_entry;
    else
      return NULL;
  }
}
/*
 * This function will be handleing the function calls as they are traversed.
 *
 * When we arrive at a function call we do the following we store the
 * parameters in registers(1,2,..13 so max 13 parameters).
 *
 * we jump to that functions label.
 *
 * Right now we just want it work with the most basic function call with no
 * function parameters.
 */
void handle_function_call(node *astnode, ErrorArray *errors, Scope *globalScope,
                          FILE *out) {

  char name[128];
  TableEntry *funcEntry = get_function_entry(astnode->scope, astnode, errors);
  if (funcEntry == NULL) {
    fprintf(debugging_info, "PROBLEM!!!\n");
    return;
  }

  int params_count = funcEntry->data.funcEntry.numfparams;
  node *paramsNode = astnode->children[0];

  if ((astnode->parent->type == dot &&
       astnode->parent->children[0] == astnode) ||
      (astnode->parent->parent->type == dot && astnode->parent->type == dot &&
       astnode->parent->children[1] == astnode)) {
    snprintf(name, sizeof(name), "%s_%s", funcEntry->scope->scopeName,
             get_name(astnode));
    fprintf(debugging_info, "created member function label %s ... \n", name);
  } else {
    snprintf(name, sizeof(name), "%s", get_name(astnode));
    fprintf(debugging_info, "created free function label %s ... \n", name);
  }
  // We load the values of the parameters into the heap portion that we have
  // reserved for this purpose

  /*
   * Note that we just load the first number of valid parameters that we know
   * are valid after this we stop, like in js.
   */
  const char *temp_register = get_next_free_register();
  const char *parameter_value_register = get_next_free_register();

  int current_storing_position = 0;
  for (int i = 0; i < paramsNode->numchildren && i < params_count; i++) {
    int paramOffset =
        handle_expression(paramsNode->children[i], errors, out, astnode->scope);

    fprintf(out, "addi %s,r0,%d\n", temp_register, current_storing_position);
    fprintf(out, "lw %s, %d(r14)\n", parameter_value_register, paramOffset);
    fprintf(out, "sw parameterstorage(%s),%s\n", temp_register,
            parameter_value_register);

    current_storing_position += 4;
  }
  free_register(parameter_value_register);
  free_register(temp_register);

  int sizeOfFunction =
      get_function_size(astnode,
                        errors); // Note that we have a function that gets the
                                 // size of a function.

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

  debugging_info = fopen("current_compile_debug.log", "w+");
  semantic_stack *stack = init_stack();
  push_node(root, stack);

  bool main_found = false;
  // We intitialize the topofstackpointer, should be done for all files.

  Scope *current_scope = root->scope; // This should be the global scope!

  while (stack->size > 0) {

    node *current = pop_node(stack);

    if (current_scope->type == FUNCTION_SCOPE && current->scope &&
        current->scope != current_scope) {
      // We need to jump back to the correct stack frame location, to do this
      // we get the address that is stored, at the top of the stack frame in
      // the first 4 bytes (word).

      current_scope = current->scope;
      fprintf(out, "lw r15, 0(r14)\n");
      fprintf(out, "jr r15\n\n\n");
    }

    if (current->scope)
      current_scope = current->scope;

    if (current->type == funcdef) {

      const char *name = get_name(current);
      char buffer[64];

      /*
       * Here we encounter problem that we do not get the correct class table
       * entry, this is what we need to go into if we are a class definition.
       *
       * We need to get the class name, we append that to the label that we are
       * creating for the function or else we will get naming conflicts.
       */
      TableEntry *func_entry =
          get_function_definition_entry(current, current_scope, errors, out);

      print_entry(func_entry, debugging_info);

      if (current->parent->type == impldef) {
        fprintf(debugging_info, "CLASS FUNCTION DEFINITION ... \n");

        const char *class_name = get_name(current->parent);
        Scope *scopePtr = current->scope;

        while (scopePtr && scopePtr->type != GLOBAL_SCOPE)
          scopePtr = scopePtr->parentScope;

        snprintf(buffer, sizeof(buffer), "%s_%s", class_name, name);
        fprintf(debugging_info, "created label %s  for class function %s.\n",
                buffer, name);
      } else {
        snprintf(buffer, sizeof(buffer), "%s", name);
      }

      fprintf(out, "align\n%s\n", buffer);
      fprintf(out, "sw 0(r14),r15\n");

      if (!func_entry) {
        fprintf(debugging_info, "ERROR NULL VALUE RETURNED!!\n");
      }

      Scope *function_scope =
          func_entry->data.funcEntry.scope; // This is causing a probblem ...
      fprintf(debugging_info, "The functions name is %s it's scope is %s...\n",
              func_entry->data.funcEntry.name, func_entry->scope->scopeName);

      fprintf(debugging_info, "entry %s has %d parameters \n", name,
              func_entry->data.funcEntry.numfparams);

      if (func_entry->data.funcEntry.numfparams > 0) {

        const char *temp_register = get_next_free_register();
        const char *value_register = get_next_free_register();

        int loading_position = 0;

        /*
         * Note that we place the function parameters in predictable locations
         * so when we retrieve them it is essentially the same order that they
         * were inserted.
         */
        for (int i = 0; i < func_entry->data.funcEntry.numfparams; i++) {

          TableEntry *fparam_entry = get_entry(
              function_scope,
              func_entry->data.funcEntry.fparamslist[i]->data.fparamEntry.name);

          int parameter_offset = fparam_entry->offset;

          fprintf(out, "addi %s,r0,%d\n", temp_register, loading_position);
          fprintf(out, "lw %s, parameterstorage(%s)\n", value_register,
                  temp_register);
          fprintf(out, "sw %d(r14),%s\n", parameter_offset, value_register);
          loading_position += 4;
        }
        free_register(temp_register);
        free_register(value_register);
      }
    }
    if (current->type == funccall) {
      handle_function_call(current, errors, globalScope, out);
    }
    if (current->type == funcdef && strcmp(get_name(current), "main") == 0) {
      main_found = true;
    }

    if (current->type == assingop) {
      handle_assignment_statement(current, errors, current->scope, out);
    }

    if (current->type == read) {
      int offset =
          handle_expression(current->children[0], errors, out, globalScope);

      if (offset) {
        fprintf(out, "addi r14,r14,-4\n");
        fprintf(out, "jl r15,read\n");
        fprintf(out, "addi r14,r14,4\n");
        fprintf(out, "sw %d(r14),r13\n", offset);
      } else {
        const char *temp_register = get_next_free_register();
        fprintf(out, "addi %s,r0,%d %%s the temp register is %s\n",
                temp_register, index_load_pointer, temp_register);

        index_store_pointer -= 4;
        if (index_store_pointer > 0) {
          index_load_pointer = index_store_pointer - 4;
        }

        fprintf(out, "lw %s,indexstorage(%s)\n", temp_register, temp_register);
        fprintf(out, "add %s,%s,r14\n", temp_register, temp_register);
        fprintf(out, "addi r14,r14,-4\n");
        fprintf(out, "jl r15,read\n");
        fprintf(out, "addi r14,r14,4\n");
        fprintf(out, "sw 0(%s),r13\n", temp_register);
      }
    }
    if (current->type == write) {

      node *valueNode = current->children[0];

      if (valueNode->type == dot) {
        fprintf(debugging_info, "DOT\n");
      }
      int offset = handle_expression(valueNode, errors, out, globalScope);
      if (offset) { // When the offset that is returned is not zero directly
                    // get from stack frame.
        fprintf(out, "lw r13, %d(r14)\n", offset);
        fprintf(out, "addi r14,r14,-%d\n", 8);
        fprintf(out, "jl r15, write\n");
        fprintf(out, "addi r14,r14, %d\n", 8);
      } else {

        fprintf(debugging_info,
                " !!!LOADING OFFSET FROM LOAD POINTER OF VALUE !!!!%d\n",
                index_load_pointer);
        const char *temp = get_next_free_register();
        fprintf(out, "addi %s,r0,%d\n", temp, index_load_pointer);

        index_load_pointer -= 4;

        if (index_store_pointer == 0)
          index_load_pointer = 0;
        else
          index_load_pointer = index_store_pointer - 4;

        fprintf(out, "lw %s,indexstorage(%s)\n", temp, temp);
        fprintf(out, "add %s,%s,r14\n", temp,
                temp); // Now we have the stackframe offset stored in temp.
        fprintf(out, "lw r13,0(%s)\n", temp);
        free_register(temp);
        fprintf(out, "addi r14,r14,-%d\n", 8);
        fprintf(out, "jl r15, write\n");
        fprintf(out, "addi r14,r14,%d\n", 8);
      }
    }

    if (current->type == ifnode) {
      handle_if_statment(current, globalScope, errors, out);
      continue;
    }
    if (current->type == returnnode) {

      node *return_value_node = current->children[0];
      int offset =
          handle_expression(return_value_node, errors, out, globalScope);

      if (offset) {
        fprintf(out, "lw r13,%d(r14)\n", offset);
        fprintf(out, "lw r15, 0(r14)\n");
        fprintf(out, "jr r15\n"); // Note that we should return to calling
                                  // function at this point.
      } else {
      }
    }
    if (current->type == whilenode) {
      handle_while_statement(current, globalScope, errors, out);
      continue;
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
    if (main_found) {
      fprintf(out, "\nentry\n");
      fprintf(out, "addi r14,r0,topaddr\n");
      fprintf(out, "addi r14,r14,-%d\n", mainFuncSize);
      fprintf(out, "jl r15,main\n");
      fprintf(out, "addi r14,r14,%d\n", mainFuncSize); // Pop of the stack.
      fprintf(out, "hlt\n");
      fprintf(out, "\n\n\n");
    }
  }

  fclose(debugging_info);
  return;
}

/*
 * For expressions what we can do is do a dfs of the root of each expression
 * tree, store the values of litvals in there corresponding memory offsets.
 * If we do not have a litval then we have a variable that itself must
 * Note that a read statement will read the value of what is passed in the
 * console into a variable. The write statement will write the variable out to
 * the console.
 *
 * Tomorrow the first thing that we should work on is the print statement and
 * read statements, this is actuall really very simple, during the time we use
 * a
 *
 * How will we deal with this?
 *
 * We will begin with our print statements and see how we can make this work?
 */

/*
 * This function will read a variable from the console and store it in the
 * correct variable.
 *
 */

/*
 * This subroutine we call whenever the read function is called.
 */

void read_subtroutine(FILE *out) {

  fprintf(out, "read\n");
  fprintf(out, "sw 0(r14),r15\n");

  const char *value_register = get_next_free_register();
  const char *stding_register = get_next_free_register();
  const char *compare_register = get_next_free_register();

  fprintf(out, "addi %s,r0,0\n", value_register);
  fprintf(out, "getdigit\n");
  fprintf(out, "getc %s\n", stding_register);
  fprintf(out, "ceqi %s,%s,10\n", compare_register, stding_register);
  fprintf(out, "subi %s,%s,48\n", stding_register, stding_register);
  fprintf(out, "bnz %s,done\n", compare_register);
  fprintf(out, "muli %s,%s,10\n", value_register, value_register);
  fprintf(out, "add %s,%s,%s\n", value_register, stding_register,
          value_register);
  fprintf(out, "j getdigit\n");
  // Note that we have to store the value in a register.

  // Store the word in the correct register and return it to the calling
  // function.
  fprintf(out, "done\n");
  fprintf(out, "add r13, r0, %s\n", value_register);
  fprintf(out, "lw r15, 0(r14)\n");
  fprintf(out, "jr r15\n");
  free_register(value_register);
  free_register(stding_register);
  free_register(compare_register);
}
/*
 * This function we call whenever we have a write statement, we treat this as
 * it's own subroutine that creates a stack frame.
 *
 * What is the actual algorithm that we will be usign to print the statement.
 * 1. we store the value in an offset in the stack frame. We will use 4(r14),
 * this makes sense because the first four bytes are for the return address
 * and the next however many bytes will store the integer that we are going to
 * print to the console.
 */
void write_subroutine(FILE *out) {

  const char *valueRegister =
      get_next_free_register(); // This will be the register that we hold the
  const char *printRegister =
      get_next_free_register(); // This register we will use to hold the
                                // actual
  const char *magnitudeRegister = get_next_free_register();
  const char *tempValRegister = get_next_free_register();

  fprintf(out, "align\n");
  fprintf(out, "write\n");
  fprintf(out, "sw 0(r14),r15\n"); // Store the return address.
  fprintf(out, "sw 4(r14), r13\n");
  fprintf(out, "lw %s, 4(r14)\n", valueRegister);
  fprintf(out, "addi %s,r0,1\n", magnitudeRegister);
  fprintf(out, "mag\n"); // This part is subroutine that will get the
                         // magnitude of the integer.
  fprintf(out, "div %s,%s,%s\n", tempValRegister, valueRegister,
          magnitudeRegister);
  fprintf(out, "cgei %s,%s,10\n", printRegister, tempValRegister);
  fprintf(out, "bz %s,print\n", printRegister);
  fprintf(out, "muli %s,%s,10\n", magnitudeRegister, magnitudeRegister);
  fprintf(out, "j mag\n");

  fprintf(out, "print\n");
  fprintf(out, "div %s,%s,%s\n", tempValRegister, valueRegister,
          magnitudeRegister);
  fprintf(out, "addi %s,%s,48\n", tempValRegister, tempValRegister);
  fprintf(out, "putc %s\n", tempValRegister);
  fprintf(out, "subi %s,%s,48\n", tempValRegister, tempValRegister);
  fprintf(out, "mul %s,%s,%s\n", printRegister, tempValRegister,
          magnitudeRegister);
  fprintf(out, "sub %s, %s, %s\n", valueRegister, valueRegister, printRegister);
  fprintf(out, "divi %s,%s,10\n", magnitudeRegister, magnitudeRegister);
  fprintf(out, "ceqi %s,%s,0\n", tempValRegister, magnitudeRegister);
  fprintf(out, "bz %s, print\n", tempValRegister);

  char *temp_reg = get_next_free_register();
  fprintf(out, "addi %s,r0, 10\n", temp_reg);
  fprintf(out, "putc %s\n", temp_reg);

  fprintf(out, "lw r15,0(r14)\n");
  fprintf(out, "jr r15\n");

  free_register(temp_reg);
  free_register(valueRegister);
  free_register(printRegister);
  free_register(magnitudeRegister);
  free_register(tempValRegister);

  fprintf(out, "\n\n\n");
}

/*
 * This function will print the instructions that will be necessary to load the
 * value for the index offset into the register passed and then retrieve the
 * value at that offset.
 */
