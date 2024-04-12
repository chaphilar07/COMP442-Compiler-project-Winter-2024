/*
 * This file defines the logic for building the AST we will inject the semantic
 * actions in the RHS of the productions so we will be inserting these into the
 * functions of the parser.
 */
#include "AST_gen.h"
#include "../../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  This function creates a leaf node in the tree, we can only have types of id,
//  intlit, floatlit and operators in this function.
//  we will use this in our match() function for our parser whenever we match a
//  terminal we will push it onto the stack.
//  Note that we should inly call this function on leaf node terminals not on
//  any other kind of terminal eg. let for example

int x = 150; // Random number used for the DOT language visualization to assign
             // nodes an id number.

node *create_leaf_node(const char *type, semantic_stack *stack,
                       const char *value, FILE *output) {
  node *temp = malloc(sizeof(node));
  if (!temp) {
    error("ERROR - create_leaf_node(): Cannot allocate memory for the node "
          "exiting");
    return NULL;
  }

  temp->node_number = x++; // Used for when we create the diagram for the tree.
  temp->numchildren = 0;
  temp->value = strdup(value);
  temp->type = type;
  temp->parent = NULL;
  temp->left_sibling = NULL;
  temp->right_sibling = NULL;
  temp->children = NULL;

  if (!stack) {
    error("ERROR create_leaf_node(): Stack has not been allocated exiting");
    return NULL;
  }

  push_node(temp, stack);

  char buffer[124];

  snprintf(buffer, sizeof(buffer),
           "Leaf token created of type %s and value %s \n", type, value);
  fprintf(output, "%s", buffer);

  return temp;
}

/* Note that when we have an aggregate token, we do not need to give it any
 * actual value like we would for a leaf node no need to add a parameter for the
 * function
 * This function takes a type in form of sting and returns a new node, gives it
 * a type of none, beacuse this is supposed to be used on aggregate nodes, these
 * nodes have types but not a value in the tree only the leaves of the tree have
 * actual values because they are actual tokens in the source code.
 */
node *init_node(const char *type) {
  if (!type) {
    error("ERROR - init_node(): Passing invalid type, exiting");
    return NULL;
  }
  node *new_node = malloc(sizeof(node));
  if (!new_node) {
    error(
        "ERROR - init_node(): Cannot allocate memory to the new node exiting");
    return NULL;
  }

  new_node->node_number = x++;
  new_node->value = NULL;
  new_node->numchildren = 0;
  new_node->type = type;
  new_node->children = NULL;
  new_node->left_sibling = NULL;
  new_node->right_sibling = NULL;
  new_node->parent = NULL;

  return new_node;
}
semantic_stack *init_stack() {
  semantic_stack *temp = malloc(sizeof(semantic_stack));
  if (!temp) {
    error("ERROR - init_stack(): Could not allocate memory for the semantic "
          "stack exiting");
    return NULL;
  }
  temp->head = NULL;
  temp->size = 0;
  return temp;
}

int push_node(node *ast_node, semantic_stack *stack) {
  if (!stack) {
    error("ERROR - push_node(): No memory has been allocated to the semantic "
          "stack");
    return 0;
  }
  if (!ast_node) {
    error("ERROR - push_node(): No memmory has been allocated to the AST node");
    return 0;
  }

  stack_node *temp = malloc(sizeof(stack_node));
  if (!temp) {
    error("ERROR - push_node(): Cannot allocate memory for a stack node, "
          "exiting");
    return -1;
  }

  temp->value = ast_node;
  stack_node *temp0 = stack->head;
  stack->head = temp;
  stack->head->next = temp0;
  stack->size += 1;
  return 1;
}

// Note that we may have namespace conflict with teh stack that we created for
// messages.
// The user must call free on the node that is returned by this function.
node *pop_node(semantic_stack *stack) {
  if (!stack) {
    error("ERROR  - pop() : Cannot pop from the stack, stack not allocated any "
          "memory");
    return NULL;
  }
  if (stack->head == NULL) {
    printf("Stack is already empty, cannot pop\n");
    return NULL;
  }
  stack_node *temp = stack->head;
  node *return_value = temp->value;
  stack->head = stack->head->next;
  stack->size -= 1;
  free(temp);
  return return_value;
}
const char *peek_stack(semantic_stack *stack) {
  if (!stack) {
    error("ERROR - peek_stack(): Stack has not been allocated any memory "
          "terminating");
    return NULL;
  }
  if (!stack->head) {
    error("ERROR - peek_stack(): Head of stack has not been allocated "
          "terminating");
    return NULL;
  }
  if (!stack->head->value) {
    error("ERROR - peek_stack(): Stack head node value has not been allocated "
          "terminating");
    return NULL;
  }
  if (!stack->head->value->type) {
    error("ERROR - peek_stack(): Stack head node has no type");
    return NULL;
  }

  const char *ret_value = stack->head->value->type;
  return ret_value;
}

// This is the pop_until() function that will pop from the stack until it, we
// take the length so we can use it later. reaches the sentinel token.
node **pop_until(const char *type, semantic_stack *stack, int *len) {
  if (!stack) {
    error(
        "ERROR - pop_until(): Cannot pop stack has not been allocated yet. \n");
    return NULL;
  }
  if (!type) {
    error("ERROR - type():The type passed is null, exiting");
    return NULL;
  }

  node **nodes = malloc(sizeof(node *));
  int current_size = 1;
  *len = 0;

  while (stack->size > 0) {
    node *temp = pop_node(stack);
    if (strcmp(temp->type, type) == 0) {
      free(temp);
      return nodes;
    }
    nodes[(*len)++] = temp;
    if ((*len) >= current_size) {
      current_size *= 2;
      nodes = realloc(nodes, sizeof(node *) * current_size);
    }
  }
  printf("%d\n", *len);
  return NULL;
}

/*
 * This function will completely empty the stack, used for debugging purposes.
 */
void empty_stack(semantic_stack *stack) {
  if (stack->size == 0) {
    error("ERROR - empty_stack(): Stack is already empty");
  }
  while (stack->size > 0) {
    node *temp = pop_node(stack);
    printf("Node type %s has value %s \n", temp->type, temp->value);
    free(temp);
  }
}

node *create_subtree(const char *type, semantic_stack *stack,
                     FILE *outputfile) {
  int len = 0;
  int current_size = 1;
  node **child_nodes = malloc(sizeof(node *));

  // Check if the stack is empty if it is we cannot create a subtree.
  if (stack->size <= 0) {
    error("ERROR - create_subtree(): Stack is already empty, exiting");
    return NULL;
  }
  while (stack->size > 0 && strcmp(peek_stack(stack), type)) {
    child_nodes[len] = pop_node(stack);
    if (child_nodes[len++] == NULL) {
      error("ERROR - create_subtree() : NULL returned for child node exiting");
      return NULL;
    }

    // double the size of the array each time, we reach the capacity.
    if (len >= current_size) {
      current_size *= 2;
      child_nodes = realloc(child_nodes, sizeof(node *) * current_size);
      if (!child_nodes) {
        error("ERROR - create_subtree(): Could not allocate memory to the "
              "arrray of "
              "child nodes exiting");
        return NULL;
      }
    }
  }

  if (peek_stack(stack) != NULL && strcmp(peek_stack(stack), type)) {
    error("ERROR - create_subtree: Could not create subtree the root node is "
          "not in the stack, exiting. "
          "found in the stack exiting");
    return NULL;
  }

  node *parent = pop_node(stack);

  if (!parent) {
    error("ERROR - create_subtree(): The parent node was not found in the "
          "stack terminating \n");
    return NULL;
  }

  if (strcmp(parent->type, type)) {
    error(
        "ERROR - create_subtree(): The parent node was not found in the stack");
    return NULL;
  }
  child_nodes = realloc(child_nodes, sizeof(node *) * len);
  // Whenever we call the pop-until function we get that we are creating a
  // create_family may as well call it in the function.
  //
  create_family(parent, child_nodes, len, type);
  fprintf(outputfile,
          "Aggregate node of type %s and value of %s with %d childen \n",
          parent->type, parent->value, parent->numchildren);
  return parent;
}

/*
 * This function creates the subtrees that will make up the AST, note that what
 * we are doing is passing the children as an array with the length to a
 * function that will then create the parent node
 *
 * The function create_subtree is the function that actually gets called in the
 * parser, this function just creates the family
 */
int create_family(node *parent, node **children, int numchildren,
                  const char *type) {
  if (children == NULL) {
    parent->numchildren = 0;
    parent->children = NULL;
    parent->node_number = x++;
  }
  parent->type = type;
  parent->children = children;
  parent->numchildren = numchildren;
  parent->node_number = x++;
  for (int i = 0; i < numchildren; i++) {
    children[i]->parent = parent;
    if (i == 0) {
      children[i]->left_sibling = children[numchildren - 1];
      children[i]->right_sibling = children[i + 1];

    } else if (i == (numchildren - 1)) {
      children[i]->left_sibling = children[i - 1];
      children[i]->right_sibling = children[0];
    } else {
      children[i]->left_sibling = children[i - 1];
      children[i]->right_sibling = children[i + 1];
    }
  }
  return 1;
}
/*Testing the functionality of the semantic stack and the functions to create
 * the AST */

// Now we will write functions that will take an AST node and then print the
// tree in dot language.
/*
 * We will try and print the tree in the dot language and then view the image in
 * a viewer
 */

/*
 * What is the algorithm that we will be using here?
 */
// lol well use a stack.

/*
 * This function prints the nodes in the tree rooted at the argument passed.
 */
void print_tree(node *root, FILE *out) {
  if (!out) {
    error("ERROR - print_tree(): Output file pointer has not been opened, "
          "exiting");
    return;
  }
  if (!root) {
    error("ERROR - print_tree(): Cannot print the tree, root is null exiting");
    return;
  }

  semantic_stack *stack = init_stack();

  push_node(root, stack);
  while (stack->size > 0) {
    node *temp = pop_node(stack);
    fprintf(out, "Node has type %s and value %s and has %d children \n",
            temp->type, temp->value, temp->numchildren);

    if (temp->numchildren > 0) {
      // Print data of each child node.
      for (int i = 0; i < temp->numchildren; i++) {
        push_node(temp->children[i], stack);
      }
    }
  }
}

void print_stack(semantic_stack *stack, FILE *output) {
  if (!stack) {
    error(
        "ERROR - print_stack(): The stack has not been allocated, terminating");
    return;
  }

  stack_node *temp = stack->head;

  while (temp != NULL) {
    node *val = temp->value;
    fprintf(output, "[%s, %s], ", val->type, val->value);
    temp = temp->next;
  }
  fprintf(output, "\n");
  return;
}

node **pop_n_nodes(int len, semantic_stack *stack) {
  if (!stack) {
    error("ERROR - pop_n_nodes(): Stack has not been allocated any memory "
          "exiting");
    return NULL;
  }
  if (len <= 0) {
    error("ERROR - pop_n_nodes(): Cannt pop zero or less nodes from the stack "
          "exiting");
    return NULL;
  }
  if (len > stack->size) {
    error("ERROR - pop_n_nodes(): Cannot pop more nodes than there are nodes "
          "on the stack terminating");
    return NULL;
  }

  node **nodes = malloc(sizeof(node *) * len);

  for (int i = 0; i < len; i++) {
    nodes[i] = pop_node(stack);
  }

  return nodes;
}

node *create_subtree_n_nodes(const char *type, int len, semantic_stack *stack) {

  node *parent = init_node(type);
  node **nodes = pop_n_nodes(len, stack);

  if (create_family(parent, nodes, len, type)) {
    return parent;
  }
  return NULL;
}

node *create_subtree_pop_until(const char *type, const char *sentinel,
                               semantic_stack *stack) {

  if (!stack) {
    error(
        "ERROR - create_subtree_pop_until() : Stack has not been allocated any "
        "memory exiting.");
    return NULL;
  }

  // This function should work correctly.
  int len = 0;
  node **nodes = pop_until(sentinel, stack, &len);

  node *parent = malloc(sizeof(node));
  if (create_family(parent, nodes, len, type)) {
    return parent;
  }

  return NULL;
}

void print_tree_dot(node *root, FILE *out) {

  if (!root) {
    error("ERROR - print_tree_dot(): Root has not been allocated");
    return;
  }

  semantic_stack *stack = init_stack();
  push_node(root, stack);
  fprintf(out, "digraph AST {\n");
  fprintf(out, "node [shape=record];\n");
  fprintf(out, "node [fontname=Sans];charset= \" UTF-8\" splines=true "
               "splines=spline rankdir =LR\n");

  while (stack->size > 0) {
    node *temp = pop_node(stack);
    fprintf(out, "%d[label=\"%s | %s\"];\n", temp->node_number, temp->type,
            temp->value);
    for (int i = 0; i < temp->numchildren; i++) {
      fprintf(out, "%d -> %d\n", temp->node_number,
              temp->children[i]->node_number);
      push_node(temp->children[i], stack);
    }
  }
  fprintf(out, "}");
}

void create_prog_tree(semantic_stack *stack, FILE *out) {
  semantic_stack *funcstack = init_stack();
  semantic_stack *structstack = init_stack();
  semantic_stack *implstack = init_stack();

  push_node(init_node("funcdef list"), funcstack);
  push_node(init_node("impldef list"), implstack);
  push_node(init_node("structdecl list"), structstack);

  while (stack->size > 1) {
    node *temp = pop_node(stack);
    if (strcmp(temp->type, "funcdef") == 0) {
      push_node(temp, funcstack);
    } else if (strcmp(temp->type, "impl") == 0) {
      push_node(temp, implstack);
    } else if (strcmp(temp->type, "struct") == 0) {
      push_node(temp, structstack);
    }
  }

  node *struclist = create_subtree("structdecl list", structstack, out);
  push_node(struclist, stack);
  node *impllist = create_subtree("impldef list", implstack, out);
  push_node(impllist, stack);
  node *funcdeflist = create_subtree("funcdef list", funcstack, out);
  push_node(funcdeflist, stack);
}

void add_child(node *parent, node *child) {
  if (!parent || !child) {
    error("add_child_as_first: One of the nodes is NULL");
    return;
  }

  // Increase the size of the children array to accommodate the new child
  node **new_children =
      realloc(parent->children, sizeof(node *) * (parent->numchildren + 1));
  if (!new_children) {
    error("add_child_as_first: Memory allocation failed");
    return;
  }
  parent->children = new_children;

  // Shift existing children to the right
  for (int i = parent->numchildren; i > 0; i--) {
    parent->children[i] = parent->children[i - 1];
  }

  // Insert the new child at the beginning
  parent->children[0] = child;
  child->parent = parent;

  // Establish sibling relationships
  if (parent->numchildren > 0) {
    child->right_sibling =
        parent->children[1]; // The former first child is now the second
    parent->children[1]->left_sibling = child;
  }
  child->left_sibling =
      parent->children[parent->numchildren]; // Assuming circular sibling links;
                                             // adjust as needed

  // If there's only one child, ensure the left and right sibling pointers are
  // correctly circular
  if (parent->numchildren == 0) {
    child->left_sibling = child;
    child->right_sibling = child;
  }

  parent->numchildren += 1;
}

// This function is used to make a node a child of the parent.

/*The main function is just for testing the functionality of the functions for
 * creating the trees, we still have problem where we try and pop we get
 * segmentation fault.
 */
/*

int main(int argc, char *argv[]) {

  FILE *ast_output = fopen("ast_output.txt", "w+");
  FILE *nodes_output = fopen("nodes_output.txt", "w+");
  FILE *stack_contents = fopen("stack_contents.txt", "w+");

  semantic_stack *stack = init_stack();

  create_leaf_node("child node 1", stack, "1", nodes_output);
  print_stack(stack, stack_contents);
  create_leaf_node("child node 2", stack, "2", nodes_output);
  print_stack(stack, stack_contents);
  create_leaf_node("child node 3", stack, "3", nodes_output);
  print_stack(stack, stack_contents);
  create_leaf_node("child node 4", stack, "4", nodes_output);
  print_stack(stack, stack_contents);
  create_leaf_node("child node 5", stack, "5", nodes_output);
  print_stack(stack, stack_contents);
  create_leaf_node("child node 6", stack, "6", nodes_output);
  print_stack(stack, stack_contents);

  // works correctly.
  node *parent = create_subtree_pop_until("parent node", "child node 1", stack);
  print_tree(parent, ast_output);
  return 0;
} */
