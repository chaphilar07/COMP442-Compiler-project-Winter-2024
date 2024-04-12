#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct msg_node {
  const char *msg;
  struct msg_node *next;

} msg_node;

typedef struct {
  msg_node *head;
  int size;
} msg_stack;

char *append_char(char *word, int length, char c);
char peek(FILE *source);
void error(const char msg[]);
char *extractFileName(const char *fullFileName);
msg_stack *init_msg_stack();
void push(msg_stack *stack, const char *msg);
const char *pop(msg_stack *stack);
void print_all_msgs(msg_stack *stack, FILE *output_file);
char *replaceSubstring(const char *original, const char *toReplace,
                       const char *replacement);

void removeSubstring(char *source, const char *substring);
#endif // !UTILS_H
