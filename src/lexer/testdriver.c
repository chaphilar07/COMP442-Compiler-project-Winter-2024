#include "../utils/utils.h"
#include "Lexer.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

  printf("Beginning of the main function ... ");
  DIR *dir;
  struct dirent *entry;

  printf("HERE\n");
  dir = opendir("tests/lexer/");
  printf("Open the tests/lexer direcotry ... \n");

  if (dir == NULL) {
    perror("Cannot open tests directory, terminating. \n");
  }

  compile_regex();
  while (((entry = readdir(dir)) != NULL)) {
    if (strstr(entry->d_name, ".src")) {
      char *file_name = entry->d_name;
      char *name = extractFileName(file_name);

      char src_path[1024];
      snprintf(src_path, sizeof(src_path), "tests/lexer/%s.src", name);
      printf("Current source file %s .... \n", src_path);
      char tokens_path[1024];
      snprintf(tokens_path, sizeof(tokens_path), "output/lexer/%s.outlextokens",
               name);
      printf("outputing to file %s .... \n", tokens_path);
      char error_path[1024];
      snprintf(error_path, sizeof(error_path), "output/lexer/%s.outlexerrors",
               name);
      printf("outputing errors to %s ... \n", error_path);

      FILE *src = fopen(src_path, "r");
      FILE *tokens = fopen(tokens_path, "w+");
      FILE *errors = fopen(error_path, "w+");

      int line = 1;
      int current_line = line;

      while (1) {
        token *temp = malloc(sizeof(token));
        temp->lexeme = NULL;
        temp->valid = 0;

        temp = get_next_token(temp, src, &line);

        if (strcasestr(temp->category, "EOF") && temp->valid == EOF) {
          printf("End of the token stream terminating. \n");
          break;
        }

        if (current_line < temp->line)
          fprintf(tokens, "\n");
        fprintf(tokens, "[%s, %s, %d]", temp->category, temp->lexeme,
                temp->line);

        if (!temp->valid)
          fprintf(errors, "[%s, %s, %d]\n", temp->category, temp->lexeme,
                  temp->line);
        current_line = temp->line;

        free(temp->lexeme);
        free(temp);
      }
    }
  }
}
