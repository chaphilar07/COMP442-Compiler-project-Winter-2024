
/*
 * This file will contain the logic for building the symbol table, to do this we
 * need access to the AST data structures and functions.
 *
 *
 * Note that we will perform some semantic checking during the building of the
 * table this includes duplicate declaration with the same id in the same scope,
 * and duplicate implications for the same class definitions.
 *
 * Note that because we are using a hash table, we have it that the entries will
 * not have any kind of order inside of the symbol tables, we cannot rely on
 * order inside of the hash table, the entires will be distributed randomly.
 */

// defines the necessary functions for the symbol table data structures.
#include "../CodeGeneration/CodeGeneration.h"
#include "../parser/AST/AST_SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "SemanticError.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TypeInfo get_type_info(node *astnode) {
  TypeInfo info = {get_type_enum(astnode), get_type_string(astnode),
                   get_arraydims(astnode), get_number_of_dims(astnode)};

  return info;
}

/*
 * This function compares the type information of two variables.
 */
bool compare_type_info(TypeInfo info1, TypeInfo info2) {
  // We want this function to to type promotion of ints -> floats when unary.

  if (info1.type != info2.type)
    return false;

  if (strcmp(info1.typeString, info2.typeString))
    return false;
  if (info1.numberofdims != info2.numberofdims)
    return false;

  // Note that we can have two variables with the same number of of dimensions
  // but they have different quantities for those dimensions so they do not have
  // to be equal.

  return true;
}

TableEntry *get_entry(Scope *scope, const char *key);

const char *get_name(node *astnode);

/*
 * This function will return the type of a scope as a string.
 */
const char *get_scope_type_string(ScopeType type) {
  if (type == GLOBAL_SCOPE)
    return "global scope";
  else if (type == CLASS_SCOPE)
    return "class scope";
  else if (type == FUNCTION_SCOPE)
    return "function scope";
  else
    return NULL;
}

unsigned int FNV1a_hash(const char *str, int size) {
  if (!str) {
    fprintf(stderr, "ERROR FNV-1a(): Cannot hash a NULL string.\n");
    return 0;
  }
  unsigned int hash = 2166136261U; // FNV-1a offset basis
  const unsigned int prime = 16777619;

  while (*str) {
    hash ^= (unsigned char)*str++;
    hash *= prime;
  }
  return hash % size;
}

const char *get_entry_type_string(EntryType type);
// This is the function that will insert the entry into the hash table.
err_code insert_entry(Scope *scope, TableEntry *entry) {
  if (!scope) {
    fprintf(stderr, "ERROR - insert_entry(): Cannot insert entry scope has not "
                    "been allocated.\n");
    return -1;
  }
  if (!scope->entries) {
    fprintf(stderr, "ERROR - insert_entry(): Cannt insert entry the table has "
                    "not been allocated\n");
    return -1;
  }
  if (!entry) {
    fprintf(stderr, "ERROR - insert_entry(): Cannot insert an entry in the "
                    "table, entry does not exist\n");
    return -1;
  }

  const char *name = NULL;

  if (entry->tableType == VARIABLE_ENTRY)
    name = entry->data.varEntry.name;
  else if (entry->tableType == FUNCDEF_ENTRY)
    name = entry->data.funcEntry.name;
  else if (entry->tableType == CLASS_ENTRY)
    name = entry->data.classEntry.name;
  else if (entry->tableType == FPARAM_ENTRY)
    name = entry->data.fparamEntry.name;

  // Now we must check if the entry is already occupied.

  TableEntry *existingEntry = get_entry(scope, name);

  unsigned int hash = FNV1a_hash(name, SIZE);
  if (existingEntry->tableType == EMPTY_ENTRY) {
    fprintf(stderr, "INSERTING %s INTO SCOPE %s ... \n", name,
            scope->scopeName);
    scope->entries[hash] = *entry;
    return ok;

  }

  else {
    return err800;
  }

  return ok;
}

/*
 * Do not have to call free on this later on.
 */
const char *get_vis_string(varvis vis) {
  if (vis == priv)
    return "private";
  else if (vis == pub)
    return "public";
  else
    return "none";
}

/*
 * These functions we will use to validate if a variable/function has been
 * declared by lookinp up it's entry in the current scope and any previous
 * scopes.
 */

TableEntry *delete_entry(Scope *scope, const char *key) {

  if (!scope) {
    fprintf(stderr, "ERROR - insert_entry(): Cannot insert entry scope has not "
                    "been allocated.\n");
    return NULL;
  }

  if (!scope->entries) {
    fprintf(stderr, "ERROR - delete_entry(): Cannot delete entry the table has "
                    "not been allocated\n");
    return NULL;
  }

  unsigned int hash = FNV1a_hash(key, SIZE);

  TableEntry *entry = &scope->entries[hash];
  free(entry);

  return NULL;
}

TableEntry *get_entry(Scope *scope, const char *key) {
  if (!scope) {
    fprintf(stderr, "ERROR - get_entry(): Cannot get entry the scope has not "
                    "been allocated\n");
    return NULL;
  }

  unsigned int hash = FNV1a_hash(key, SIZE);

  if (&(scope->entries[hash]) == NULL) {
    fprintf(stderr,
            "No entry has been inserted into the table with key %s, exiting \n",
            key);
    return NULL;
  }
  return &(scope->entries[hash]);
}
// These functions are for the oepration of the Scope stack we will need a
// pop, push and peek function.

ScopeStack *init_scope_stack() {
  ScopeStack *stack = malloc(sizeof(ScopeStack));
  if (!stack) {
    fprintf(stderr, "ERROR - init_scope_stack(): Cannot create a new scope "
                    "stack exiting");
    return NULL;
  }

  stack->size = 0;
  stack->head = NULL; // Set the head to NULL allocate when we push the first
                      // scope node.
  return stack;
}
// This pushes a scope onto the current scope.
int push_scope(Scope *scope, ScopeStack *stack) {
  if (!stack) {
    fprintf(stderr, "ERROR - push_scope(): Cannot push a scope onto the stack, "
                    "stack is NULL, terminating\n");
    return -1;
  }

  // Get pointer to the current head.
  ScopeNode *current_head = stack->head;

  ScopeNode *new_head = malloc(sizeof(ScopeNode));
  new_head->scope = scope;
  stack->head = new_head;
  stack->head->next = current_head;
  stack->size += 1;

  return 1;
}

// Pops a node off of the top of the  stack.
Scope *pop_scope(ScopeStack *stack) {
  if (!stack) {
    fprintf(stderr, "ERROR - pop_scope(): Cannot pop node off of stack that "
                    "has not been allocated memory. \n");
    return NULL;
  }

  if (stack->head == NULL && stack->size <= 0) {
    fprintf(stderr, "ERROR - pop_scope(): Cannot pop a node off the stack "
                    "stack is already empty.\n");
    return NULL;
  }

  ScopeNode *scopeNodePtr = stack->head;
  Scope *scopePtr = scopeNodePtr->scope;
  stack->head = stack->head->next;
  free(scopeNodePtr);
  stack->size -= 1;

  return scopePtr;
}

// This returns a pointer to the head node of the stack, so we can modify the
// head of the stack, by using peek.
Scope *peek_scope(ScopeStack *stack) {
  if (!stack) {
    fprintf(stderr, "ERROR - pop_scope(): Cannot pop node off of stack that "
                    "has not been allocated memory. \n");
    return NULL;
  }

  return stack->head->scope;
}

// The hashing function for the symbol table, we will use well known hashin
// function, greater description of the algorithm found at
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

/*
 * This function will create a scope, note that we can have the parentScope
 * parameter be NULL because if it is NULL, we are creating global scope.
 */
Scope *init_scope(Scope *parentScope, const char *scopeName, ScopeType type) {
  Scope *scope = malloc(sizeof(Scope));

  if (!scope) {
    fprintf(stderr, "ERROR - init_scope(): Cannot allocate memory for a new "
                    "scope terminating");
    return NULL;
  }

  scope->type = type;
  scope->parentScope = parentScope;
  scope->scopeName = scopeName;

  scope->entries = malloc(sizeof(TableEntry) * SIZE);
  for (int i = 0; i < SIZE; i++) {
    scope->entries[i].tableType =
        EMPTY_ENTRY; // We denote that the entry will be empty.
  }

  if (!scope->entries) {
    fprintf(stderr,
            "ERROR - init_scope(): Cannot allocate memory for the symbol table "
            "of scope %s, exiting",
            scope->scopeName);
    return NULL;
  }

  return scope;
}
// First we will define the functions for creating the entries, to do this we
// will have helper functions that will help us get the name, type etc for a
// certain entry in the table, all of our functions will return a TableEntry,
// we will take a node as input to the function.

/*
 * This function will create a scope.
 * Note that we will also use this function to create the tables.
 */

/*These functions will get certain fields that are necessary for most table
 * entries, these are a string for the type, the type enum.
 */
// This function will get the name from a variable, struct or impl node.
const char *get_name(node *astnode) {
  if (!astnode) {
    fprintf(stderr,
            "ERROR - get_name(): Cannot get name of null node, terminating\n");
    return NULL;
  }

  if (astnode->numchildren <= 0) {
    fprintf(stderr,
            "ERROR - get_name(): Node does not have a name, terminating\n");
    return NULL;
  }

  const char *name = NULL;
  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i] == NULL)
      continue;
    if (astnode->children[i]->type == identifier)
      name = astnode->children[i]->value;
  }

  return name;
}

const char *get_type_from_enum(LangType type) {
  if (type == INT_TYPE)
    return "integer";
  else if (type == FLOAT_TYPE)
    return "float";
  else if (type == ID_TYPE)
    return "id type";
  else if (type == NONE_TYPE)
    return "none";
  else if (type == VOID_TYPE)
    return "void";
  else
    return "PROBLEM";
}
// This gets the type enum, this can be useful for faster checking and easier
// to understand.
LangType get_type_enum(node *astnode) {

  if (!astnode) {
    fprintf(stderr, "ERROR - get_type_enum(): Cannot get name of null node, "
                    "terminating\n");
    return 0;
  }
  int numdims = astnode->numchildren;

  for (int i = 0; i < numdims; i++) {
    if (astnode->children[i]->type == type) {
      if (strcmp(astnode->children[i]->value, "integer") == 0)
        return INT_TYPE;
      else if (strcmp(astnode->children[i]->value, "float") == 0)
        return FLOAT_TYPE;
      else if (!strcmp(astnode->children[i]->value, "void"))
        return VOID_TYPE;
      else
        return ID_TYPE;
    }
  }
  return 0;
}

const char *get_entry_type_string(EntryType type) {
  if (type == FPARAM_ENTRY)
    return "fparam";
  else if (type == VARIABLE_ENTRY)
    return "variable";
  else if (type == FUNCDEF_ENTRY)
    return "function";
  else if (type == CLASS_ENTRY)
    return "class";
  else
    return "none";
}

// This function will get the type of a varible or a function definition node.
const char *get_type_string(node *astnode) {

  if (!astnode) {
    fprintf(stderr, "ERROR - get_tyep_string(): Cannot get name of null node, "
                    "terminating\n");
    return NULL;
  }

  if (astnode->numchildren <= 0) {
    fprintf(stderr, "ERROR - get_type_string(): Node does not have a name, "
                    "terminating\n");
    return NULL;
  }

  // This will
  const char *typeString;

  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == type)
      typeString = astnode->children[i]->value;
  }

  return typeString;
}

// This function will get the array dimensions for a node of variable or
// fparam type.
int *get_arraydims(node *astnode) {
  if (!astnode) {
    fprintf(stderr, "ERROR - get_arraydims(): Cannot get arraydims node is "
                    "null, terminating\n");
    return NULL;
  }

  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == arraydims) {

      int numdims = astnode->children[i]->numchildren;
      int *arrdims = malloc(sizeof(int) * numdims);

      if (numdims > 0) {
        for (int j = 0; j < numdims; j++) {

          arrdims[j] = atoi(astnode->children[i]->children[j]->value);
        }
        return arrdims;
      } else {
        return NULL;
      }
    }
  }

  return NULL;
}
int get_number_of_dims(node *astnode) {

  if (!astnode) {
    fprintf(stderr, "ERROR - get_arraydims(): Cannot get arraydims node is "
                    "null, terminating\n");
    return -1;
  }

  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == arraydims) {
      return astnode->children[i]->numchildren;
    }
  }

  return 0;
}

varvis get_vis(node *astnode) {

  if (!astnode) {
    fprintf(stderr, "ERROR - get_arraydims(): Cannot get arraydims node is "
                    "null, terminating\n");
    return -1;
  }

  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == visibility) {
      if (strcmp(astnode->children[i]->value, "private") == 0)
        return priv;
      else if (strcmp(astnode->children[i]->value, "public") == 0)
        return pub;
    }
  }
  return none;
}

void print_entry(TableEntry *entry, FILE *out) {
  if (!entry) {
    fprintf(stderr, "ERROR - get_entry_string(): Cannot print the entry the "
                    "entry is null, exiting\n");
    return;
  }

  if (entry->tableType == VARIABLE_ENTRY) {
    if (entry->data.varEntry.vis != none) {
      fprintf(out, "%s ", get_vis_string(entry->data.varEntry.vis));
    }
    fprintf(out, "var %s %s", entry->data.varEntry.name,
            entry->data.varEntry.type.typeString);
    for (int i = entry->data.varEntry.type.numberofdims - 1; i >= 0; i--) {
      if (entry->data.varEntry.type.arraydims[i] > 0)
        fprintf(out, "[%d]", entry->data.varEntry.type.arraydims[i]);
      else
        fprintf(out, "[]");
    }
    fprintf(out, "\tsize: %d\toffset: %d", entry->size, entry->offset);
    fprintf(out, "\n");
  }
  if (entry->tableType == FPARAM_ENTRY) {
    fprintf(out, "fparam %s %s", entry->data.fparamEntry.name,
            entry->data.fparamEntry.type.typeString);
    for (int i = entry->data.fparamEntry.type.numberofdims - 1; i >= 0; i--) {
      if (entry->data.fparamEntry.type.arraydims[i] > 0)
        fprintf(out, "[%d]", entry->data.fparamEntry.type.arraydims[i]);
      else
        fprintf(out, "[]");
    }
    fprintf(out, "\tsize: %d\toffset: %d", entry->size, entry->offset);
    fprintf(out, "\n");
  }
  if (entry->tableType == FUNCDEF_ENTRY) {

    if (entry->data.funcEntry.vis == pub || entry->data.funcEntry.vis == priv) {
      fprintf(out, "%s ", get_vis_string(entry->data.funcEntry.vis));
    }
    fprintf(out, "func %s -> %s : ", entry->data.funcEntry.name,
            entry->data.funcEntry.returnType.typeString);
    for (int i = entry->data.funcEntry.numfparams - 1; i >= 0; i--) {
      fprintf(out, "%s %s",
              entry->data.funcEntry.fparamslist[i]->data.fparamEntry.name,
              entry->data.funcEntry.fparamslist[i]
                  ->data.fparamEntry.type.typeString);
      for (int j = entry->data.funcEntry.fparamslist[i]
                       ->data.fparamEntry.type.numberofdims -
                   1;
           j >= 0; j--) {
        if (entry->data.funcEntry.fparamslist[i]
                ->data.fparamEntry.type.arraydims[j] > 0) {
          fprintf(out, "[%d]",
                  entry->data.funcEntry.fparamslist[i]
                      ->data.fparamEntry.type.arraydims[j]);
        } else {
          fprintf(out, "[]");
        }
      }
      if (i > 0)
        fprintf(out, ",");
    }

    fprintf(out, "\tsize: %d\treturn type size: %d", entry->size,
            entry->data.funcEntry.returnTypeSize);
    fprintf(out, "\n");
  }
  if (entry->tableType == CLASS_ENTRY) {
    fprintf(out, "class %s inherits ", entry->data.classEntry.name);
    for (int i = entry->data.classEntry.inheritsCount - 1; i >= 0; i--) {
      fprintf(out, "%s, ",
              entry->data.classEntry.inheritedScopes[i]->scopeName);
    }
    fprintf(out, "\tsize: %d\toffset: %d", entry->size, entry->offset);
    fprintf(out, "\n");
  }
}

/*
 * This returns an array of pointers to the inherited scopes of a class.
 * functions as it should.
 */

// END OF HELPERS
// These Functions will create the table entries when we encounter specific
// nodes in the AST.
//
//
TableEntry *create_fparam_entry(node *astnode, Scope *currentScope) {
  if (!astnode) {
    fprintf(stderr, "ERROR - get_arraydims(): Cannot get arraydims node is "
                    "null, terminating\n");
    return NULL;
  }
  TableEntry *entry = malloc(sizeof(TableEntry));
  if (!entry) {
    fprintf(stderr, "ERROR - create_fparam_entry(): Cannot allocate memory for "
                    "fparam entry, exiting.\n");
    return NULL;
  }

  entry->line = astnode->line;

  entry->scope = currentScope;
  entry->tableType = FPARAM_ENTRY;

  entry->data.fparamEntry.type = get_type_info(astnode);
  entry->data.fparamEntry.name = get_name(astnode);

  unsigned int typeSize;

  if (entry->data.fparamEntry.type.type == INT_TYPE)
    typeSize = 4;
  else if (entry->data.fparamEntry.type.type == FLOAT_TYPE)
    typeSize = 8;
  else if (entry->data.fparamEntry.type.type == ID_TYPE) {
    Scope *scopePtr = currentScope; // Take copy of the currentScopes pointer.

    while (scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
    }
    typeSize =
        get_entry(scopePtr, entry->data.fparamEntry.type.typeString)
            ->size; // We get the size of the class being declared, so we will
                    // need to create the size of the classes as we go.
  } else {
    // Error!
    return NULL;
  }

  // Get the total size of the variable.
  unsigned int varSize = 1 * typeSize;
  if (entry->data.fparamEntry.type.numberofdims > 0 &&
      entry->data.fparamEntry.type.arraydims) {
    for (int i = 0; i < entry->data.fparamEntry.type.numberofdims; i++) {
      varSize *= entry->data.fparamEntry.type.arraydims[i];
    }
  }
  entry->size = varSize;

  return entry;
}

// This gets an array of function parameters.
TableEntry **get_fparams_list(node *astnode, Scope *currentscope) {

  if (!astnode) {
    fprintf(stderr, "ERROR - get_arraydims(): Cannot get arraydims node is "
                    "null, terminating\n");
    return NULL;
  }

  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == fparamslist) {

      TableEntry **fparams =
          malloc(sizeof(TableEntry *) *
                 astnode->children[i]
                     ->numchildren); // We will have to call free on this later
                                     // on, we will go through the symbol tables
                                     // and free all dynamic memory.

      for (int j = 0; j < astnode->children[i]->numchildren; j++) {

        fparams[j] = create_fparam_entry(astnode->children[i]->children[j],
                                         currentscope);
      }

      return fparams;
    }
  }

  return NULL;
}

int get_fparam_count(node *astnode) {

  if (!astnode) {
    fprintf(stderr, "ERROR - get_arraydims(): Cannot get arraydims node is "
                    "null, terminating\n");
    return -1;
  }

  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == fparamslist) {
      return astnode->children[i]->numchildren;
    }
  }

  return 0;
}

TableEntry *create_variable_entry(node *astnode, Scope *currentScope,
                                  ErrorArray *errors) {

  if (!astnode) {
    fprintf(stderr, "ERROR - get_arraydims(): Cannot get arraydims node is "
                    "null, terminating\n");
    return NULL;
  }

  TableEntry *entry = malloc(sizeof(TableEntry));
  if (!entry) {
    fprintf(stderr, "ERROR - create_variable_entry(): Cannot allocate memory "
                    "for new entry exiting\n");
    return NULL;
  }

  entry->line = astnode->line;
  entry->scope = currentScope;
  entry->tableType = VARIABLE_ENTRY;

  entry->data.varEntry.type = get_type_info(astnode);
  entry->data.varEntry.name = get_name(astnode);

  entry->data.varEntry.vis = get_vis(astnode);

  unsigned int typeSize;

  if (entry->data.varEntry.type.type == INT_TYPE)
    typeSize = 4;
  else if (entry->data.varEntry.type.type == FLOAT_TYPE)
    typeSize = 8;
  else if (entry->data.varEntry.type.type == ID_TYPE) {
    Scope *scopePtr = currentScope; // Take copy of the currentScopes pointer.

    while (scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
    }
    typeSize =
        get_entry(scopePtr, entry->data.varEntry.type.typeString)
            ->size; // We get the size of the class being declared, so we will
                    // need to create the size of the classes as we go.
  } else {
    // Error!
    return NULL;
  }

  // Get the total size of the variable.
  unsigned int varSize = 1 * typeSize;
  if (entry->data.varEntry.type.numberofdims > 0 &&
      entry->data.varEntry.type.arraydims) {
    for (int i = 0; i < entry->data.varEntry.type.numberofdims; i++) {
      varSize *= entry->data.varEntry.type.arraydims[i];
    }
  }
  entry->size = varSize;

  return entry;
}

/*
 * This function will get the number of inherited scopes.
 */

TableEntry *create_func_entry(node *astnode, Scope *scope) {

  if (!astnode) {
    fprintf(stderr, "ERROR - get_arraydims(): Cannot get arraydims node is "
                    "null, terminating\n");
    return NULL;
  }

  TableEntry *entry = malloc(sizeof(TableEntry));
  if (!entry) {
    fprintf(stderr, "ERROR - create_func_entry(): Cannot allocate function "
                    "entry, exiting\n");
    return NULL;
  }

  if (astnode->type == funcdecl && scope->type == CLASS_SCOPE) {

    entry->data.funcEntry.defined = false;
    entry->data.funcEntry.memberFunc = true;
    entry->data.funcEntry.vis = get_vis(astnode);

  } else if (astnode->type == funcdef && scope->type == GLOBAL_SCOPE) {

    entry->data.funcEntry.defined = true;
    entry->data.funcEntry.memberFunc = false;
    entry->data.funcEntry.vis = none;
  }

  entry->line = astnode->line;
  entry->scope = scope;
  entry->tableType = FUNCDEF_ENTRY;
  entry->data.funcEntry.returnType = get_type_info(astnode);
  entry->data.funcEntry.name = get_name(astnode);
  entry->data.funcEntry.scope =
      init_scope(scope, entry->data.funcEntry.name, FUNCTION_SCOPE);
  entry->data.funcEntry.fparamslist = get_fparams_list(astnode, scope);
  entry->data.funcEntry.numfparams = get_fparam_count(astnode);

  // We insert the function parameters into the scope right away.
  for (int i = 0; i < entry->data.funcEntry.numfparams; i++) {
    entry->data.funcEntry.fparamslist[i]->tableType = FPARAM_ENTRY;

    // We insert the entry into the functions scope.
    insert_entry(entry->data.funcEntry.scope,
                 entry->data.funcEntry.fparamslist[i]);
  }

  // We also need the size of the return type.
  TypeInfo returnInfo = entry->data.funcEntry.returnType;

  // Note that a function cannot return an array type only unary.
  if (returnInfo.type == INT_TYPE)
    entry->data.funcEntry.returnTypeSize = 4;
  else if (returnInfo.type == FLOAT_TYPE)
    entry->data.funcEntry.returnTypeSize = 8;
  else if (returnInfo.type == ID_TYPE) {

    Scope *scopePtr = scope;

    while (scopePtr->type != GLOBAL_SCOPE) {
      scopePtr = scopePtr->parentScope;
    }

    entry->data.funcEntry.returnTypeSize =
        get_entry(scopePtr, entry->data.funcEntry.returnType.typeString)->size;

  } else if (returnInfo.type == VOID_TYPE) {
    entry->data.funcEntry.returnTypeSize = 0;
  } else {
    entry->data.funcEntry.returnTypeSize = 0;
  }

  return entry;
}
// current function and assign it the parent scope.

TableEntry *create_class_entry(node *astnode, Scope *currentScope,
                               ErrorArray *arr) {

  if (!astnode) {
    fprintf(stderr, "ERROR - create_class_entry(): Cannot create a class entry "
                    "null node, exiting\n");
    return NULL;
  }

  if (astnode->type != structdecl) {
    fprintf(stderr,
            "ERROR - create_class_entry: Cannot create class node for node of "
            "type %s , terminating\n",
            get_node_type_string(astnode));
    return NULL;
  }

  TableEntry *entry = malloc(sizeof(TableEntry));
  if (!entry) {
    fprintf(stderr, "ERROR - create_class_entry(): Cannot allocate memory for "
                    "a new class entry exiting.\n");
    return NULL;
  }

  entry->line = astnode->line;
  entry->scope = currentScope;
  entry->tableType = CLASS_ENTRY;
  entry->data.classEntry.name = get_name(astnode);
  entry->data.classEntry.scope =
      init_scope(currentScope, entry->data.classEntry.name, CLASS_SCOPE);

  // We need to get the inheritedScopes and the number of inheritedScopes, how
  // can we do this?
  // No seperate functions we will do it all here.

  node *inheritsNode = NULL;

  for (int i = 0; i < astnode->numchildren; i++) {
    if (astnode->children[i]->type == inherits)
      inheritsNode = astnode->children[i];
  }

  if (inheritsNode == NULL) {
    entry->data.classEntry.inheritsCount = 0;
    entry->data.classEntry.inheritedScopes = NULL;
  } else {
    entry->data.classEntry.inheritsCount = 0;
    for (int i = 0; i < inheritsNode->numchildren; i++) {
      EntryType type =
          get_entry(currentScope, inheritsNode->children[i]->value)->tableType;

      if (type == CLASS_ENTRY) {

        entry->data.classEntry.inheritedScopes =
            realloc(entry->data.classEntry.inheritedScopes,
                    sizeof(Scope *) * (entry->data.classEntry.inheritsCount++));
        entry->data.classEntry
            .inheritedScopes[entry->data.classEntry.inheritsCount - 1] =
            get_entry(currentScope, inheritsNode->children[i]->value)
                ->data.classEntry.scope;
      } else {
        fprintf(stderr, "SEMANTIC ERROR: Cannot inherit a class that does not "
                        "exist exiting\n");
        insert_error(arr, create_error(inheritsNode->children[i]->value, err200,
                                       inheritsNode->line));
      }
    }
  }
  return entry;
}

// ------------------ END CREATION FUNCTIONS ---------------------------

/*
 * This funtion will create the whole table, we will create a global scope
 * that has all of the class and free functions in it's scope the entries in
 * the symbol table of the scope will contain the pointers to the other nested
 * scopes.
 */

/*
 * This is the main function that we will call for creating the symbol tables,
 * we have 2 stacks one for nodes and one for Scopes. We have two parameters a
 * node that is the root of the AST that we are currently in and a pointer to
 * a file for debugging data.
 *
 * We will also perform some semantic checks during this phase, so we will
 * hvae to also check ndoes of type:
 * 1. var
 * 2. funccall
 */

FILE *temp;
Scope *create_program_scope(node *root, FILE *out, void *arr) {

  char buffer[124];
  snprintf(buffer, sizeof(buffer), "nodes%d.txt", rand());
  temp = fopen(buffer, "w+");

  ErrorArray *errors = (ErrorArray *)arr;

  // We will use a typecast not the safest thing to do but it will work for
  // now.

  semantic_stack *node_stack = init_stack();
  ScopeStack *scope_stack = init_scope_stack();

  push_node(root, node_stack);
  Scope *globalScope = init_scope(
      NULL, "Global", GLOBAL_SCOPE); // We want to keep a pointer to the global
                                     // scope can be useful for quick lookups.
  push_scope(globalScope, scope_stack);
  Scope *current_scope = peek_scope(scope_stack);

  while (node_stack->size > 0) {

    node *current = pop_node(node_stack);
    current_scope = peek_scope(scope_stack);

    // while we build the symbol table.
    while (current->type == sentinel) {

      /*
       * What we can do here is get the sizes for the classes and for the
       * functions, we will get them as we build the tables makes things
       * simpler.
       */
      Scope *scopePtr = pop_scope(scope_stack);
      if (scopePtr->type == CLASS_SCOPE || scopePtr->type == FUNCTION_SCOPE) {

        TableEntry *tempEntry =
            get_entry(scopePtr->parentScope, scopePtr->scopeName);

        unsigned int runningSum = 0;

        for (int i = 0; i < SIZE; i++) {

          if (scopePtr->entries[i].tableType == VARIABLE_ENTRY ||
              scopePtr->entries[i].tableType == FPARAM_ENTRY) {

            scopePtr->entries[i].offset = runningSum;
            runningSum += scopePtr->entries[i].size;
          }
        }

        tempEntry->size = runningSum;
      }
      current_scope = peek_scope(scope_stack);

      if (node_stack->size > 0)
        current = pop_node(node_stack);
      else
        break;
    }

    if (current_scope == NULL)
      fprintf(stderr, "TRYING TO ASSIGN CURRENT SCOPE TO NULL!\n");

    current->scope = current_scope; // We set the scopes of the AST nodes

    // Note that because we want to have the function parameters also
    // be entries in the function scopes table, we must make the same
    // kind of

    // Note for dots we will traverse the tree before we assign the scope so we
    // must assign scopes now rather than later.
    if (current->type == dot) {
      semantic_stack *stack = init_stack();
      push_node(current, stack);

      while (stack->size > 0) {
        node *currentNode = pop_node(stack);

        currentNode->scope = current_scope;

        if (currentNode->numchildren > 0) {
          for (int i = 0; i < currentNode->numchildren; i++) {
            push_node(currentNode->children[i], stack);
          }
        }
      }
    }

    if (current->type == var && current->scope != NULL) {

      err_code code = validate_lookup(current, errors, globalScope);

      if (code == err111) {
        insert_error(errors,
                     create_error(get_name(current), err111, current->line));
      }
    }

    if (current->type == vardecl || current->type == fparam) {

      LangType currentType = get_type_enum(current);
      if (currentType == ID_TYPE) { // This does not work correctly!

        const char *typeName = get_type_string(current);
        TableEntry *testEntry = get_entry(globalScope, typeName);

        if (testEntry->tableType != CLASS_ENTRY) {
          insert_error(errors,
                       create_error(get_name(current), err205, current->line));
        }
      }
      err_code code = insert_entry(
          current_scope, create_variable_entry(current, current_scope, errors));

      if (code != ok) {
        insert_error(errors,
                     create_error(get_name(current), code, current->line));
      }

    } else if (current->type == structdecl) {

      TableEntry *entry = create_class_entry(current, current_scope, errors);

      err_code code = insert_entry(current_scope, entry);
      if (code != ok) {
        insert_error(errors,
                     create_error(get_name(current), code, current->line));
      }

      push_scope(entry->data.classEntry.scope, scope_stack);
      push_node(init_node(sentinel), node_stack);

      for (int i = 0; i < current->numchildren; i++)
        push_node(current->children[i], node_stack);
      continue;

    } else if (current->type == funcdecl) {

      LangType currentType = get_type_enum(current);
      if (currentType == ID_TYPE) { // This does not work correctly!

        const char *typeName = get_type_string(current);
        TableEntry *testEntry = get_entry(globalScope, typeName);

        if (testEntry->tableType != CLASS_ENTRY) {
          insert_error(errors,
                       create_error(get_name(current), err205, current->line));
        }
      }

      err_code code = insert_entry(current_scope,
                                   create_func_entry(current, current_scope));

      if (code != ok) {
        insert_error(errors,
                     create_error(get_name(current), code, current->line));
      }

    } else if (current->type == funcdef) {

      if (current_scope->type == GLOBAL_SCOPE) {

        TableEntry *entry = create_func_entry(current, current_scope);
        err_code code = insert_entry(current_scope, entry);
        if (code != ok) {
          insert_error(errors,
                       create_error(get_name(current), code, current->line));
        }
        push_node(init_node(sentinel), node_stack);
        push_scope(entry->data.funcEntry.scope, scope_stack);

        node *funcbodynode = NULL;

        for (int i = 0; i < current->numchildren; i++) {
          if (current->children[i]->type == funcbody)
            funcbodynode = current->children[i];
        }

        funcbodynode->scope = current_scope;
        for (int i = 0; i < funcbodynode->numchildren; i++) {
          push_node(funcbodynode->children[i], node_stack);
        }
      }
      if (current_scope->type == CLASS_SCOPE) {

        const char *name = get_name(current);
        TableEntry *entry = get_entry(current_scope, name);

        if (entry->tableType == FUNCDEF_ENTRY) {

          entry->data.funcEntry.defined =
              true; // Note that we have defined the function.
          push_scope(entry->data.funcEntry.scope, scope_stack);
          push_node(init_node(sentinel), node_stack);

          node *funcbodynode = NULL;

          node *fparamsListNode = NULL;
          for (int i = 0; i < current->numchildren; i++) {
            if (current->children[i]->type == fparamslist) {
              fparamsListNode = current->children[i];
            }
          }

          // We also need to compare the parameters and the types of the
          // parameters.
          // Create a temporary function entry for the impls function definition
          // and compare the parameters and return type.
          TableEntry *tempFuncEntry = create_func_entry(current, current_scope);

          if (entry->data.funcEntry.numfparams !=
              fparamsListNode->numchildren) {
            insert_error(errors, create_error(get_name(current), err1410,
                                              current->line));
          } else {

            for (int i = 0; i < fparamsListNode->numchildren; i++) {
              TypeInfo info1 =
                  entry->data.funcEntry.fparamslist[i]->data.fparamEntry.type;
              TypeInfo info2 = tempFuncEntry->data.funcEntry.fparamslist[i]
                                   ->data.fparamEntry.type;

              if (!compare_type_info(info1, info2)) {
                insert_error(errors, create_error(get_name(current), err1411,
                                                  current->line));
              }
            }
          }

          TypeInfo typeExpected = entry->data.funcEntry.returnType;
          TypeInfo typeReceived = tempFuncEntry->data.funcEntry.returnType;

          if (typeReceived.type != typeExpected.type) {
            insert_error(errors, create_error(get_name(current), err1412,
                                              current->line));
          }
          for (int i = 0; i < current->numchildren; i++) {
            if (current->children[i]->type == funcbody)
              funcbodynode = current->children[i];
          }

          funcbodynode->scope = current_scope;
          for (int i = 0; i < funcbodynode->numchildren; i++) {
            push_node(funcbodynode->children[i], node_stack);
          }

        } else {
          // We will "stem" the tree here, what we will do is remove the link to
          // this tree at the parent.

          /*
           * Want to to here is "cut" the fundef node from the rest of the tree,
           * to do this we will get the parent node find the child node of the
           * parent node that corresponds to this function definition nodes and
           * we wll set it to NULL.
           *
           * This should work correctly but it is causing seg faults when we are
           *
           */

          int line = current->line;
          node *parent = current->parent;
          remove_child(
              parent,
              current); // This function removes the child from the tree.

          FILE *temp = fopen("dottest.txt", "w+");
          print_tree_dot(root, temp);

          insert_error(errors, create_error(name, err203, line));
        }
      }

    } else if (current->type == impldef) {

      const char *name = get_name(current);

      int hash = FNV1a_hash(name, SIZE);

      if (current_scope->entries[hash].tableType == EMPTY_ENTRY) {
        // We should also stem this from the tree, it is not necessary.

        int line = current->line;
        node *parent = current->parent;
        remove_child(parent, current);

        insert_error(errors, create_error(get_name(current), err101, line));
        continue;
      }
      if (current_scope->entries[hash].tableType != CLASS_ENTRY) {

        int line = current->line;
        node *parent = current->parent;
        remove_child(parent, current);

        insert_error(errors, create_error(get_name(current), err101, line));
        // We report a semantic error do not insert the function.
        continue;
      }

      push_scope(current_scope->entries[hash].data.classEntry.scope,
                 scope_stack);

      push_node(init_node(sentinel), node_stack);

      for (int i = 0; i < current->numchildren; i++)
        push_node(current->children[i], node_stack);

    } else {

      for (int i = current->numchildren - 1; i >= 0; i--)
        push_node(current->children[i], node_stack);
    }
  }

  /*
   * Perform the second pass and then calcuate the size and offsets for the code
   * generation.
   */
  second_pass_type_check(root, globalScope,
                         errors); // Now this will be working correctly.

  return globalScope;
}

// This function works correctly right now for filling out the symbol tables
// of each scope, now we need to perform the semantic checks.

/*
 * Prints the contents of a scope out to a file, if we have a entry that has a
 * nested scope, we recursively call the function to that scope.
 */

/*
 * This function will be used to print the scopes of a program, note that we
 * do not have any pretty tables etc, we print a very basic representation of
 * the tables, that follows the following:
 * 1. if the entry starts a new scope, eg a function or a struct we will start
 * the scope of that entry immediately
 * We will use tabes to denote the level of nesting.
 */
void print_scope(Scope *scope, FILE *out, unsigned int tabs) {

  if (!scope) {
    fprintf(stderr, "ERROR - print_scope(): Scope passed is null exiting\n");
    return;
  }
  if (!out) {
    fprintf(stderr,
            "ERROR - print_scope(): File passed has not been opened exiting\n");
    return;
  }
  for (int i = 0; i < tabs; i++)
    fprintf(out, "\t\t\t");

  fprintf(out, "BEGINNING OF TABLE FOR:  %s \"%s\"\n\n",
          get_scope_type_string(scope->type), scope->scopeName);

  int count = 1;
  for (int i = 0; i < SIZE; i++) {
    if (scope->entries[i].tableType != EMPTY_ENTRY) {
      for (int i = 0; i < tabs; i++)
        fprintf(out, "\t\t\t");
      fprintf(out, "Table Entry %d of \"%s\":", count++, scope->scopeName);
      print_entry(&(scope->entries[i]), out);
      fprintf(out, "\n");
      if (scope->entries[i].tableType == CLASS_ENTRY) {
        print_scope(scope->entries[i].data.classEntry.scope, out, tabs + 1);
      }
      if (scope->entries[i].tableType == FUNCDEF_ENTRY) {
        print_scope(scope->entries[i].data.funcEntry.scope, out, tabs + 1);
      }
    }
  }

  for (int i = 0; i < tabs; i++)
    fprintf(out, "\t\t\t");
  fprintf(out, "END OF TABLE FOR SCOPE %s \"%s\"\n\n",
          get_scope_type_string(scope->type),

          scope->scopeName);
}
/*
 * Now we may have to refactor the functions that create entries into the table
 * so that they also take an ErrorArray.
 */

/*
 * Free symobl tables, this function will free all of the memory that we
 * allocated dynamically we will call this when we are done using the memory.
 */

int free_scopes(Scope *globalScope) {
  if (!globalScope) {
    fprintf(
        stderr,
        "ERROR - free_scopes(): Cannot free a scope that is null, exiting.\n");
    return -1;
  }

  if (globalScope->parentScope != NULL) {
    fprintf(stderr, "EXPECTING A GLOBAL SCOPE CANNOT CALL THIS FUNCTION ON A "
                    "NON-GLOBAL SCOPE.\n");
    return -1;
  }

  return 1;
}
