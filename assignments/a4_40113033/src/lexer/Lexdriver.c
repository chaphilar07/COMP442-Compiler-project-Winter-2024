#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

  compile_regex();
  int line = 1;

  token *temp = NULL;

  FILE *positive_source = fopen("tests/lexer/lexpositivegrading.src", "r");
  FILE *positive_tokens =
      fopen("output/lexer/lexpositivegrading.outlextokens", "w+");
  FILE *positive_errors =
      fopen("output/lexer/lexpositivegrading.outlexerrors", "w+");
  int current_line = line;

  while (1) {
    temp = malloc(sizeof(token));
    temp = get_next_token(temp, positive_source, &line);

    if (strcasestr(temp->category, "EOF") && temp->valid == EOF) {
      printf("End of the token stream terminating. \n");
      break;
    }

    if (temp->line > current_line) {
      fprintf(positive_tokens, "\n");
    }

    fprintf(positive_tokens, "[%s, %s, %d] ", temp->category, temp->lexeme,
            temp->line);

    if (!temp->valid)
      fprintf(positive_errors, "[%s, %s, %d]\n", temp->category, temp->lexeme,
              temp->line);

    current_line = temp->line;
    free(temp->lexeme);
    free(temp);
    temp = NULL;
  }
  fclose(positive_tokens);
  fclose(positive_source);
  fclose(positive_errors);
  current_line = 1;
  line = 1;
  FILE *negative_source = fopen("tests/lexer/lexnegativegrading.src", "r");
  FILE *negative_tokens =
      fopen("output/lexer/lexnegativegrading.outlextokens", "w+");
  FILE *negative_errors =
      fopen("output/lexer/lexnegativegrading.outlexerrors", "w+");

  while (1) {

    temp = malloc(sizeof(token));
    temp->lexeme = NULL;
    temp->valid = 0;

    temp = get_next_token(temp, negative_source, &line);

    if (strcasestr(temp->category, "EOF") && temp->valid == EOF) {
      printf("End of the token stream terminating. \n");
      break;
    }

    if (temp->line > current_line) {
      fprintf(negative_tokens, "\n");
    }

    fprintf(negative_tokens, "[%s, %s, %d]", temp->category, temp->lexeme,
            temp->line);

    if (temp->valid == 0)
      fprintf(negative_errors, "[%s, %s, %d]\n", temp->category, temp->lexeme,
              temp->line);
    current_line = temp->line;
    free(temp->lexeme);
    free(temp);
    temp = NULL;
  }
  fclose(negative_tokens);
  fclose(negative_source);
  fclose(negative_errors);

  free_regexes();
  return 0;
}
