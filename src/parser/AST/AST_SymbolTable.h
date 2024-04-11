/*
 * Header file for the AST generation code
 * will provide the  e
 */

#ifndef AST_SYMBOL_TABLE_DOT_H
#define AST_SYMBOL_TABLE_DOT_H

// This is the enum for the type of the node in the AST, will have to refactor
// the rest of the code.
#include <stdbool.h>
#include <stdio.h>

#define SIZE 2048

// Type of the scope.
typedef enum ScopeType { GLOBAL_SCOPE, FUNCTION_SCOPE, CLASS_SCOPE } ScopeType;

// Type of the table entries.
typedef enum EntryType {
  LITVAL_ENTRY,
  TEMP_ENTRY,
  VARIABLE_ENTRY,
  FUNCDEF_ENTRY,
  CLASS_ENTRY,
  FPARAM_ENTRY,
  EMPTY_ENTRY,

} EntryType;

typedef struct LITVAL_ENTRY {

  const char *id;
  const char *value;
} LitValEntry;

typedef struct TempVarEntry {
  const char *name;
} TempVarEntry;
// Type in the actual langauge.
typedef enum LangType {

  NONE_TYPE,
  VOID_TYPE,
  INT_TYPE,
  FLOAT_TYPE,
  ID_TYPE

} LangType;

// Visibility for members.
typedef enum visibility { none, priv, pub } varvis;

typedef struct TypeInfo {

  LangType type;
  const char *typeString;
  int *arraydims;
  int numberofdims;

} TypeInfo;

typedef struct VariableEntry {

  TypeInfo type;
  const char *name;
  varvis vis; // If there is no visibility we set this to none type.

} VariableEntry;

typedef struct FparamEntry {

  int id; // Used for the order that it was inserted used to get the offset.
  TypeInfo type;
  const char *name;

} FparamEntry;

typedef struct FuncdefEntry {

  unsigned int returnTypeSize;
  varvis vis;
  bool defined;
  bool memberFunc;

  TypeInfo returnType;
  const char *name;
  struct TableEntry **fparamslist;
  int numfparams;
  struct Scope *scope;

} FuncdefEntry;

typedef struct ClassEntry {

  const char *name;
  struct Scope *scope;
  struct Scope **inheritedScopes; // Links to each of the inherited scopes.
  int inheritsCount;

} ClassEntry;

typedef struct Scope {

  ScopeType type;
  struct Scope *parentScope;
  const char *scopeName;
  struct TableEntry *entries; // We hold the Symbol table in a Scope , the
                              // symbol table will be a hash table.

} Scope;

// Our table entry should contain something called an offset, the offset will
// the difference with the top of the stack frame

typedef struct TableEntry {

  /*
   * We need to keep track of the size of the entry and the offset of the entry
   * inside of the table.
   */
  unsigned int size; // This is the actual size of the entry do not confuse with
                     // the offset, we will use this when we reserve bytes.
  unsigned int offset; // The offset within the current symbol table.

  EntryType tableType;
  Scope *scope;

  int line; // We will keep the line of the entry that corresponds to the entry
            // in the actual source code.
  union {
    LitValEntry litval;
    TempVarEntry TempVarEntry;
    VariableEntry varEntry;
    FuncdefEntry funcEntry;
    ClassEntry classEntry;
    FparamEntry fparamEntry;
  } data;

} TableEntry;

// We will use a stack to keep track of the scopes while traversing the tree to
// construct the AST.
typedef struct ScopeNode {
  Scope *scope;
  struct ScopeNode *next;

} ScopeNode;

typedef struct ScopeStack {
  ScopeNode *head;
  int size;

} ScopeStack;

void print_scope(Scope *scope, FILE *out, unsigned int tabs);

typedef enum nodetype {

  relop,
  dot,
  inherits,
  intnum,
  floatnum,
  identifier,
  type,
  addop,
  multop,
  assingop,
  structdecl,
  funcdef,
  funcdecl,
  vardecl,
  visibility,
  whilenode,
  ifnode,
  returnnode,
  sign,
  dimlist,
  aparamslist,
  aparam,
  arraydims,
  fparam,
  fparamslist,
  funcdeflist,
  impldeflist,
  structdecllist,
  impldef,
  sentinel,
  prog,
  None,
  emptydim,
  relexpr,
  notnode,
  funccall,
  var,
  statblock,
  funcbody,
  read,
  write

} NodeType;

NodeType get_nodeType_enum(const char *name);
// This is the node data type for the AST tree.

// We embed the scope into the AST nodes.
typedef struct node {
  const char *entryName; // We use this to store a "key" for the corresponding
                         // table entry, used for code gen expression handling.

  Scope *scope;      // The scope that the node the AST belongs to.
  unsigned int line; // We need to keep the line number for semantic reports.
  NodeType type;
  struct node **children;
  int numchildren;
  struct node *right_sibling;
  struct node *left_sibling;
  struct node *parent;
  const char *value;
  int node_number;
} node;

// We will be using an explicit stack instead of using the program's stack
// frame.
typedef struct _node {
  struct _node *next;
  node *value;
} stack_node;

typedef struct stack {
  stack_node *head;
  int size;
} semantic_stack;

node *create_leaf_node(NodeType type, semantic_stack *stack, const char *value,
                       FILE *output, unsigned int line);

int push_node(node *ast_node, semantic_stack *stack);

node *pop_node(semantic_stack *stack);

semantic_stack *init_stack();

node *init_node(NodeType type);

int create_family(node *parent, node **children, int numchildren,
                  NodeType type);
const char *get_node_type_string(node *current);

void add_child(node *parent, node *child);

node *create_subtree(NodeType type, semantic_stack *stack, FILE *outputfile);
void print_tree(node *root, FILE *out);

NodeType peek_stack(semantic_stack *stack);
void print_stack(semantic_stack *stack, FILE *output);

node *create_subtree_pop_until(NodeType type, NodeType sentinelNode,
                               semantic_stack *stack, unsigned int line);

node *create_subtree_n_nodes(NodeType type, int len, semantic_stack *stack,
                             unsigned int line);
void print_tree_dot(node *root, FILE *out);

Scope *create_program_scope(node *root, void *arr);

void empty_stack(semantic_stack *stack);

void create_prog_tree(semantic_stack *stack, FILE *out);

TableEntry *delete_entry(Scope *scope, const char *key);

TableEntry *get_entry(Scope *scope, const char *key);

ScopeStack *init_scope_stack();

Scope *pop_scope(ScopeStack *stack);

Scope *peek_scope(ScopeStack *stack);

Scope *init_scope(Scope *parentScope, const char *scopeName, ScopeType type);

const char *get_name(node *astnode);

LangType get_type_enum(node *astnode);

const char *get_type_string(node *astnode);

int *get_arraydims(node *astnode);

int get_number_of_dims(node *astnode);

varvis get_vis(node *astnode);

void print_entry(TableEntry *entry, FILE *out);

Scope **get_inherited_scopes(node *astnode, Scope *globalScope);

TableEntry *create_fparam_entry(node *astnode, Scope *currentScope,
                                int fparamNumber);

TableEntry **get_fparams_list(node *astnode, Scope *currentscope);

int get_fparam_count(node *astnode);

int get_inherits_count(node *astnode);

TableEntry *create_func_entry(node *astnode, Scope *scope);

int push_scope(
    Scope *scope,
    ScopeStack *stack); // This function pushes a scope onto the scope stack.

const char *get_type_from_enum(LangType type);
bool compare_type_info(TypeInfo info1, TypeInfo info2);

int free_tree(node *root);
void free_entire_tree(node *root); // This function will free all of the dynamic
                                   // memory starting from the root.

TypeInfo get_type_info(node *astnode);

void remove_child(node *parent, node *child);

char *random_id();

int get_function_size(node *astnode, void *errors);

int get_class_size(const char *className, Scope *globalScope);

int insert_entry(Scope *scope, TableEntry *entry);
#endif // !AST_GEN_H
