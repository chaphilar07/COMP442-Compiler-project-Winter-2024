/*
 * Header file for the AST generation code
 * will provide the  e
 */

#ifndef AST_GEN_H
#define AST_GEN_H
#include <stdio.h>

/* We us a union for the value of the token, this is because the value of the
 * token can be an int a float or a string, thus union is the most efficient way
 * of representing this.
 */

// This enum will be used to enhance the AST nodes for when we are traversing
// the tree, this will be used
typedef enum nodetype {

  vardecl,
  funcdecl,
  funcdef,
  impldef,
  structdecl,
  dot,
  assignop,
  sign,
  intnum,
  floatnum,
  identifier,
  classtype,
  fparamslist,
  dimlist,
  arraydims,
  fparam,

} nodetype;

// This is the node data type for the AST tree.
typedef struct node {
  nodetype asttype; // This will provide a safer way of constructing the nodes,
                    // also allows direct comparison.
  struct node **children;
  int numchildren;
  struct node *right_sibling;
  struct node *left_sibling;
  struct node *parent;
  const char *type;
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

node *create_leaf_node(const char *type, semantic_stack *stack,
                       const char *value, FILE *output);
int push_node(node *ast_node, semantic_stack *stack);

node *pop_node(semantic_stack *stack);

semantic_stack *init_stack();

node *init_node(const char *type);

void add_child(node *parent, node *child);
node *create_subtree(const char *type, semantic_stack *stack, FILE *outputfile);
void print_tree(node *root, FILE *out);
int create_family(node *parent, node **children, int numchildren,
                  const char *type);

const char *peek_stack(semantic_stack *stack);
void print_stack(semantic_stack *stack, FILE *output);

node *create_subtree_n_nodes(const char *type, int len, semantic_stack *stack);

node *create_subtree_pop_until(const char *type, const char *sentinel,
                               semantic_stack *stack);

void print_tree_dot(node *root, FILE *out);

void create_prog_tree(semantic_stack *stack, FILE *out);

void empty_stack(semantic_stack *stack);
#endif // !AST_GEN_H
