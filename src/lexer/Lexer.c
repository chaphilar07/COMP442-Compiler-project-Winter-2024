/*
 * This is a simple Lexical analyzer for a front end of a compiler, the lexical
 * analyzer reads the file character by character and creates a stream of tokens
 * from the file.
 */
#include "Lexer.h"
#include "../utils/utils.h"
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

// These are the patterns for the regular expressions.
const char *float_string = "^([1-9][[:digit:]]*|0)\\.(0|([[:digit:]]*[1-9]))(["
                           "eE][+-]?(0|([1-9][[:digit:]]*)))?$";
const char *id_string = "^[[:alpha:]][[:alnum:]_]*$";
const char *int_string = "^([1-9][[:digit:]]*|0)$";

// Regular expressions for checking the validity of tokens, one for each type of
// token.
regex_t int_regex;
regex_t id_regex;
regex_t float_regex;

// This function is used to determine if the next character in the file stream
// is a valid character.
// Note that _ is not a valid start symbol for any token in the lexical spec so
// we do not include it for a valid symbol.
int is_valid_char(char c) {
  if (c == '+' || c == '-' || c == '=' || c == '/' || c == ';' || c == ':' ||
      c == '!' || c == '&' || c == '>' || c == '<' || c == ',' || c == '[' ||
      c == ']' || c == '{' || c == '}' || c == '(' || c == ')' || c == '*' ||
      c == '|' || isalnum(c) || c == '.') {
    return 1;
  } else
    return 0;
}

int compile_regex() {

  if (regcomp(&id_regex, id_string, REG_EXTENDED) != 0) {
    perror("Could not compile regular expression terminating");
    return 1;
  }
  if (regcomp(&int_regex, int_string, REG_EXTENDED) != 0) {
    perror("Could not compile regular expression terminating");
    return 1;
  }

  if (regcomp(&float_regex, float_string, REG_EXTENDED) != 0) {
    perror("Could not compile regular expression terminating");
    return 1;
  }

  return 0;
}
void free_regexes() {
  regfree(&int_regex);
  regfree(&id_regex);
  regfree(&float_regex);
}

const char *KEYWORDS[] = {"if",     "else",    "then",    "impl",     "func",
                          "return", "integer", "void",    "while",    "let",
                          "struct", "read",    "self",    "inherits", "var",
                          "float",  "public",  "private", "write"};

int is_keyword(char *lexeme) {
  if (strcasecmp(lexeme, "if") == 0 || strcasecmp(lexeme, "else") == 0 ||
      strcasecmp(lexeme, "then") == 0 || strcasecmp(lexeme, "impl") == 0 ||
      strcasecmp(lexeme, "func") == 0 || strcasecmp(lexeme, "return") == 0 ||
      strcasecmp(lexeme, "integer") == 0 || strcasecmp(lexeme, "void") == 0 ||
      strcasecmp(lexeme, "while") == 0 || strcasecmp(lexeme, "let") == 0 ||
      strcasecmp(lexeme, "struct") == 0 || strcasecmp(lexeme, "self") == 0 ||
      strcasecmp(lexeme, "inherits") == 0 || strcasecmp(lexeme, "var") == 0 ||
      strcasecmp(lexeme, "float") == 0 || strcasecmp(lexeme, "write") == 0 ||
      strcasecmp(lexeme, "read") == 0 || strcasecmp(lexeme, "public") == 0 ||
      strcasecmp(lexeme, "private") == 0) {
    return 1;
  }

  return 0;
}

const char *get_keyword(char *word) {
  for (int i = 0; i < 19; i++) {
    if (!strcasecmp(word, KEYWORDS[i])) {
      return KEYWORDS[i];
    }
  }
  return NULL;
}

// The get_next_token function returns the next token in the file, we read
// from the file each time that we extract the next token.
// This function will assume that the caller has allocated memory for the token.
// The function will dynamically allocate the string for the lexeme.
token *get_next_token(token *temp, FILE *source, int *line) {
  temp->lexeme = NULL;
  char c;

  int len = 0;
  while (1) {
    c = fgetc(source);

    // Advance hte character in file until we reach a non-whitespace character,
    // always check for the EOF.
    while (c != EOF && isspace(c) && len == 0) {
      if (c == '\n')
        *line += 1;
      c = fgetc(source);
    }

    // If we arrive at the EOF and we have not encountered a meaningful symbol
    // we return NULL to indicate the EOF to the caller.
    if (c == EOF && len == 0) {
      temp->category = "eof";
      temp->valid = EOF;
      temp->lexeme = "eof";
      temp->line = *line;
      return temp;
    }

    if (!is_valid_char(c) && !isspace(c)) {
      temp->lexeme = append_char(temp->lexeme, len, c);
      temp->category = "invalidchar";
      temp->line = *line;
      temp->valid = 0;

      return temp;
    }
    if (c != EOF && isalpha(c)) {
      while (c != EOF && (isalnum(c) || c == '_')) {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        c = fgetc(source);
      }
      temp->line = *line;

      if (is_keyword(temp->lexeme)) {
        temp->category = get_keyword(temp->lexeme);
        temp->valid = 1;
      } else if (regexec(&id_regex, temp->lexeme, 0, NULL, 0) == 0) {
        temp->category = "id";
        temp->valid = 1;
      } else {
        temp->category = "invalidid";
        temp->valid = 0;
      }
      if (!isspace(c) || c == EOF)
        fseek(source, -1, SEEK_CUR);
      if (c == '\n')
        *line += 1;
      return temp;
    } // End of ids and keywords
    else if (c != EOF && ispunct(c)) {
      if (c == '{') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        temp->category = "lcubr";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == '}') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        temp->category = "rcubr";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == '(') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "lpar";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == ')') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "rpar";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == '[') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "lsqbr";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == ']') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "rsqbr";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == ':') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        temp->category = "colon";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == ';') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "semi";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == '!') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "not";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == '|') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "or";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == ',') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        temp->valid = 1;
        temp->line = *line;
        temp->category = "comma";
        return temp;
      } else if (c == '&') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "and";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == '*') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "mult";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == '+') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "plus";
        temp->valid = 1;
        temp->line = *line;
        return temp;
      } else if (c == '-') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        temp->valid = 1;
        temp->line = *line;
        if (peek(source) == '>') {
          temp->category = "arrow";
          c = fgetc(source);
          temp->lexeme = append_char(temp->lexeme, len, c);
          len += 1;
        } else
          temp->category = "minus";
        return temp;
      } else if (c == '=') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        temp->line = *line;
        temp->valid = 1;

        if (peek(source) == '=') {
          c = fgetc(source);
          temp->lexeme = append_char(temp->lexeme, len, c);
          temp->category = "eq";
        } else
          temp->category = "assign";
        return temp;

      } else if (c == '<') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        temp->line = *line;
        temp->valid -= 1;
        if (peek(source) == '>') {
          c = fgetc(source);
          temp->lexeme = append_char(temp->lexeme, len, c);
          temp->category = "neq";

        } else if (peek(source) == '=') {
          c = fgetc(source);
          temp->lexeme = append_char(temp->lexeme, len, c);
          len += 1;
          temp->category = "leq";
        } else
          temp->category = "gt";
        return temp;
      } else if (c == '>') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        temp->line = *line;
        temp->valid = 1;

        if (peek(source) == '=') {
          temp->category = "geq";
          c = fgetc(source);
          temp->lexeme = append_char(temp->lexeme, len, c);
          len += 1;
        } else
          temp->category = "lt";

        return temp;
      } else if (c == '.') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        temp->category = "dot";
        temp->line = *line;
        temp->valid = 1;
        return temp;
      } else if (c == '/') {
        temp->lexeme = append_char(temp->lexeme, len, c);
        len += 1;
        temp->line = *line;
        if (peek(source) == '/') {
          temp->category = "inlinecmt";
          c = fgetc(source);
          temp->lexeme = append_char(temp->lexeme, len, c);
          len += 1;
          while ((c = fgetc(source)) != '\n') {
            if (c == '\r')
              continue;
            if (c == '\n')
              break;
            temp->lexeme = append_char(temp->lexeme, len, c);
            len += 1;
          }
          *line += 1;
          temp->valid = 1;
          return temp;
        } else if (peek(source) == '*') {
          c = fgetc(source);
          temp->lexeme = append_char(temp->lexeme, len, c);
          len += 1;
          temp->category = "blockcmt";
          int nesting = 1;
          int nesting_match = 0;
          int newlines = 0;
          while ((c = fgetc(source)) != EOF) {
            if (c == '\r') {
              continue;
            }

            if (c == '\n') {
              temp->lexeme = append_char(temp->lexeme, len, '\\');
              len += 1;
              temp->lexeme = append_char(temp->lexeme, len, 'n');
              len += 1;
              newlines += 1;
              continue;
            }

            temp->lexeme = append_char(temp->lexeme, len, c);
            len += 1;

            if (c == '/' && peek(source) == '*') {
              c = fgetc(source);
              temp->lexeme = append_char(temp->lexeme, len, c);
              len += 1;
              nesting += 1;
            }

            if (c == '*' && peek(source) == '/') {
              c = fgetc(source);
              temp->lexeme = append_char(temp->lexeme, len, c);
              len += 1;
              nesting_match += 1;

              if (nesting_match == nesting)
                break;
            }
          }

          temp->line = *line;
          *line += newlines;
          temp->valid = 1;
          return temp;
        } else {
          temp->category = "div";
          temp->valid = 1;
          return temp;
        }
      }
    } // End of punct
    else if (c != EOF && isdigit(c)) {
      temp->lexeme = append_char(temp->lexeme, len, c);
      len += 1;
      temp->line = *line;

      int float_num = 0;
      int exponentiation = 0;

      if (c == '0') {
        if (peek(source) == '.') {
          c = fgetc(source);
          if (peek(source) != EOF && isdigit(peek(source))) {
            float_num = 1;
            temp->lexeme = append_char(temp->lexeme, len, c);
            len += 1;
          }
        } else {
          temp->category = "intnum";
          temp->valid = 1;
          return temp;
        }
      }

      if (!float_num) {
        while ((c = fgetc(source)) != EOF && (isdigit(c) || c == '.')) {
          if (c == '.' && float_num != 0) {
            fseek(source, -1, SEEK_CUR);
          }
          temp->lexeme = append_char(temp->lexeme, len, c);
          len += 1;
          if (c == '.' && float_num == 0) {
            float_num = 1;
            break;
          }
        }
      }

      if (float_num) {
        int trailing_zeroes = 0;
        int decimal_places = 0;
        while ((c = fgetc(source)) != EOF &&
               (isdigit(c) || c == 'e' || c == 'E')) {
          temp->lexeme = append_char(temp->lexeme, len, c);
          len += 1;

          if ((c == 'e' || c == 'E') && exponentiation == 0) {
            exponentiation = 1;
            if (peek(source) == '+' || peek(source) == '-') {
              c = fgetc(source);
              temp->lexeme = append_char(temp->lexeme, len, c);
              len += 1;

              if (peek(source) == '0') {
                c = fgetc(source);
                temp->lexeme = append_char(temp->lexeme, len, c);
                len += 1;
                temp->valid = 1;
                temp->category = "floatnum";
                return temp;
              }
              while ((c = fgetc(source)) != EOF && isdigit(c)) {
                temp->lexeme = append_char(temp->lexeme, len, c);
                len += 1;
              }
              break;
            } else if (peek(source) == '0') {
              c = fgetc(source);
              temp->lexeme = append_char(temp->lexeme, len, c);
              len += 1;
              temp->category = "floatnum";
              temp->valid = 1;
              return temp;
            } else if (c != EOF && isdigit(c)) {
              temp->lexeme = append_char(temp->lexeme, len, c);
              len += 1;
            } else {
              if (isdigit(peek(source)) && c != '0') {
                c = fgetc(source);
                temp->lexeme = append_char(temp->lexeme, len, c);
                len += 1;
                while ((c = fgetc(source)) != EOF && isdigit(c)) {
                  temp->lexeme = append_char(temp->lexeme, len, c);
                  len += 1;
                }
              }
            }

          } else if ((c == 'e' || c == 'E') && exponentiation != 0) {
            fseek(source, -1, SEEK_CUR);
            break;
          }
          decimal_places++;
          if (c == '0') {
            if (trailing_zeroes == 0)
              trailing_zeroes = 1;
            else
              trailing_zeroes += 1;
          } else
            trailing_zeroes = 0;
        }

        // We check if we parsed any trailing zeroes if there are we remove
        // them.
        // We must make sure that the file is pointing at the next valid
        // character and that the lexeme does not have the additional zeroes
        // attached.
        //
        if (trailing_zeroes && !exponentiation && decimal_places > 1) {

          // in the lexical spec , 3.0 is a valid float, so we have
          if (trailing_zeroes == decimal_places) {

            len -= trailing_zeroes - 2;
            printf("Trailing zeroes same as decimal places\n");
            while (trailing_zeroes > 1) {
              c = fseek(source, -1, SEEK_CUR);
              trailing_zeroes -= 1;
            }
          } else {

            len -= trailing_zeroes - 1;
            while (trailing_zeroes != 0) {
              c = fseek(source, -1, SEEK_CUR);
              trailing_zeroes -= 1;
            }
          }

          char *word = realloc(temp->lexeme, sizeof(char) * (len));
          temp->lexeme = word;
          temp->lexeme[len - 1] = '\0';
        }
        if (c == '\n')
          *line += 1;
        if (!isspace(c))
          fseek(source, -1, SEEK_CUR);

        if (!regexec(&float_regex, temp->lexeme, 0, NULL, 0)) {
          temp->valid = 1;
          temp->category = "floatnum";
        } else {
          temp->category = "invalidnum";
          temp->valid = 0;
        }

        return temp;
      } else {
        if (c == '\n')
          *line += 1;
        if (!isspace(c))
          fseek(source, -1, SEEK_CUR);

        if (regexec(&int_regex, temp->lexeme, 0, NULL, 0) == 0) {
          temp->category = "intnum";
          temp->valid = 1;
        } else {
          temp->category = "invalidnum";
          temp->valid = 0;
        }

        return temp;
      }

    } // End of numerical
  }
}

/*
 * Note for freeing memory later on, we will need to free, LEXEME, in the parser
 * we give this value to the node->value (const char *), we must free this when
 * we are done.\
 *
 * We will refactor the category so that we are using an enum.
 *
 * The refactor should be rather straightforward.
 */
