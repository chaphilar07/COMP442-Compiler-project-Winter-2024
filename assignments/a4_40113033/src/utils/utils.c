#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removeSubstring(char *source, const char *substring) {
  char *match;
  int len = strlen(substring);

  while ((match = strstr(source, substring))) {
    // Check if the character before the substring is a space or the start of
    // the string
    if ((match == source) || (*(match - 1) == ' ')) {
      // Check if the character after the substring is a space or the end of the
      // string
      if (*(match + len) == ' ' || *(match + len) == '\0') {
        // Additional 1 for the space before the substring
        memmove(match, match + len + (*(match + len) == ' ' ? 1 : 0),
                strlen(match + len) + 1);
        continue; // Check if there are more occurrences
      }
    }
    // If we are here, it means the substring is not standalone. Break the loop.
    break;
  }
  return;
}

char *append_char(char *word, int length, char c) {
  char *temp = realloc(word, sizeof(char) * (length + 2));
  if (!temp) {
    perror("Cannot allocate memory, terminating");
    return NULL;
  }

  word = temp;
  word[length] = c;
  word[length + 1] = '\0';

  return word;
}

char peek(FILE *source) {

  if (source == NULL) {
    perror("Cannot read from the file, terminating");
    exit(1);
  }
  char d = fgetc(source);
  fseek(source, -1, SEEK_CUR);

  return d;
}

void error(const char msg[]) { fprintf(stderr, "%s\n", msg); }

char *extractFileName(const char *fullFileName) {
  char *period = strrchr(fullFileName, '.');
  if (!period || period == fullFileName) {
    return NULL;
  }
  size_t length = period - fullFileName;
  char *fileName = malloc(length + 1);
  if (fileName) {
    strncpy(fileName, fullFileName, length);
    fileName[length] = '\0';
  }
  return fileName;
}

// These functions and data structures are for printing the output of the
// parser.

msg_stack *init_msg_stack() {
  msg_stack *new_stack = malloc(sizeof(msg_stack));
  if (new_stack == NULL) {
    error(
        "ERROR - init_msg_stack() : Cannot allocate memory for message stack");
    return NULL;
  }

  new_stack->size = 0;
  new_stack->head = NULL;

  return new_stack;
}

void push(msg_stack *stack, const char *msg) {
  msg_node *new_node = malloc(sizeof(msg_node));
  if (!new_node) {
    error("ERROR - push() : cannot allocate memory for the new node");
    return;
  }
  new_node->msg = msg;
  new_node->next = stack->head;
  stack->head = new_node;
  return;
}

const char *pop(msg_stack *stack) {
  if (!stack) {
    error("ERROR - pop() : stack has not been allocated any memory");
    return NULL;
  }
  if (stack->head == NULL) {
    printf("STACK IS EMPTY \n");
    return NULL;
  }

  const char *return_string = stack->head->msg;

  msg_node *temp = stack->head;

  stack->head = stack->head->next;
  free(temp);

  return return_string;
}

void print_all_msgs(msg_stack *stack, FILE *output_file) {
  if (!stack) {
    error("ERROR - print_all_msgs() : Stack has not been allocated any memory");
    return;
  }

  const char *temp = pop(stack);

  while (temp != NULL) {
    fprintf(output_file, "%s\n", temp);
    printf("%s\n", temp);
    temp = pop(stack);
  }

  return;
}

char *findExactMatch(const char *haystack, const char *needle) {
  int needleLen = strlen(needle);
  const char *p = haystack;
  while ((p = strstr(p, needle)) != NULL) {
    // Check the character before needle
    const char *before = (p == haystack) ? NULL : (p - 1);
    // Check the character after needle
    const char *after = (*(p + needleLen) == '\0') ? NULL : (p + needleLen);

    // Check surrounding characters
    if ((before == NULL || isspace((unsigned char)*before) ||
         *before == '\0') &&
        (after == NULL || isspace((unsigned char)*after) || *after == '\0')) {
      return (char *)p; // Found a match with exact boundaries
    }
    p += needleLen; // Move past the current found location
  }
  return NULL; // No match found
}

char *replaceSubstring(const char *original, const char *toReplace,
                       const char *replacement) {
  // Find the substring to replace (if it exists).
  const char *position = strstr(original, toReplace);
  if (position == NULL) {
    // The substring to replace was not found; return a copy of the original.
    return strdup(original);
  }

  // Calculate new string length and allocate memory for it.
  size_t newStrLen = strlen(original) - strlen(toReplace) + strlen(replacement);
  char *newStr = (char *)malloc(newStrLen + 1); // +1 for null terminator
  if (newStr == NULL) {
    perror("Unable to allocate memory for new string");
    exit(EXIT_FAILURE);
  }

  // Copy the part of the original string before the substring to replace.
  size_t leadingSegmentSize = position - original;
  strncpy(newStr, original, leadingSegmentSize);

  // Copy the replacement string into the new string.
  strcpy(newStr + leadingSegmentSize, replacement);

  // Copy the remaining part of the original string after the replaced part.
  strcpy(newStr + leadingSegmentSize + strlen(replacement),
         position + strlen(toReplace));

  return newStr;
}

/*
int main(int argc, char *argv[]) {

  msg_stack *stack = init_msg_stack();
  push(stack, "Hello");
  push(stack, "world!");
  push(stack, "computer");

  FILE *src = fopen("output.txt", "w+");
  print_all_msgs(stack, src);

  return 0;
}
*/

// The main function here is just for testing the functionality of the
