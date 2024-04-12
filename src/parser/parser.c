/*
 * This file includes the logic for parseing the file, the main functionality
 * will be in the parse_file() function which performs the task of parseing and
 * printing the derivation to a file.
 */

#include "../lexer/Lexer.h"
#include "../utils/utils.h"
#include "AST/AST_SymbolTable.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Uses too much global state right now must parameratize functions instead
 * but changing the program to minimize the global state may be challenging.
 */

// These variables are for the parser.
token *lookahead = NULL;
FILE *src = NULL;
FILE *derivation_output = NULL;
FILE *error_output = NULL;
FILE *rule_output = NULL;
int line;
msg_stack *derivation_msg_stack = NULL;
char *derivation_string = NULL;

// These variables are for the AST generation.
semantic_stack *stack = NULL;
FILE *ast_output = NULL;
FILE *nodes_output = NULL;
FILE *stack_contents = NULL;

int success = true;
void updateDerivation(char **derivation, const char *nonTerminal,
                      const char *expansion) {
  if (strcmp(expansion, "") == 0) {
    removeSubstring(derivation_string, nonTerminal);
  } else {
    char *newDerivation = replaceSubstring(*derivation, nonTerminal, expansion);
    if (newDerivation != NULL) {
      free(*derivation);           // Free the old derivation string
      *derivation = newDerivation; // Update the pointer to the new string
    }
  }
  fprintf(derivation_output, "%s\n", derivation_string);
}

// This will maintain the current derivation, we update this with each rule that
// we apply

void print_rule(const char *rule) {
  if (derivation_output == NULL) {
    error("ERROR - print_rule() : derivation output file has not been opened.");
    return;
  }

  if (!derivation_msg_stack) {
    error("ERROR - print_rule() : derivation message stack has not been opened "
          "... ");
    return;
  }
  push(derivation_msg_stack, rule);
  return;
}

void syntax_error(const char *err) {
  if (error_output == NULL) {
    error("ERROR - syntax_error() : syntax error output file has not been "
          "opened.");
  }

  fprintf(error_output,
          "SYNTAX ERROR UNEXPECTED TOKEN \'%s\': %s, line: %d \t Error "
          "Message: %s \n",
          lookahead->lexeme, lookahead->category, line, err);
  return;
}

// This function initilizes the lookahead token to the first token in the
// source file returns -1 if the parth is invald or the lookahead is not assign
// to NULL.
int initLookahead(const char *path) {
  src = fopen(path, "r"); // We open the source file for reading.

  if (lookahead != NULL) {
    error("ERROR - initLookahead: lookahead already allocated.");
    return -1;
  }

  if (src == NULL) {
    error("ERROR - initLookahead: source file not open.");
    return -1;
  }
  line = 1; // Set the line number to one.

  lookahead = malloc(sizeof(token)); // Allocate memory for the token.
  if (!lookahead) {
    error("ERROR - initLookahead: cannot allocate memory for the token");
    return false;
  } else
    lookahead = get_next_token(lookahead, src, &line);

  while (strcasecmp(lookahead->category, "inlinecmt") == 0 ||
         strcasecmp(lookahead->category, "blockcmt") == 0) {

    free(lookahead);
    lookahead = NULL;

    lookahead = malloc(sizeof(token));
    lookahead = get_next_token(lookahead, src, &line);
  }

  printf("lookahead initialized \n");
  return true;
}

// This function compares the lookahead to the type that is passed.
int compare(const char *type) {
  if (type == NULL) {
    error("ERROR - compare(): NULL string passed to function.");
    return -1;
  }
  if (lookahead == NULL || lookahead->category == NULL) {
    error("ERROR - compare() : lookahead token has no value.");
    return -1;
  }

  if (!strcasecmp(type, lookahead->category))
    return true;
  else
    return false;
}

// This is the match function we advance the token for each terminal that we
// encounter.
int match(const char *type) {
  if (lookahead == NULL) {
    error("ERROR match() : lookahead token is NULL \n");
  }

  if (strcasecmp(lookahead->category, type) == 0) {

    if (strcmp("id", lookahead->category) == 0 ||
        strcmp("intnum", lookahead->category) == 0 ||
        strcmp("floatnum", lookahead->category) == 0 ||
        strcmp("integer", lookahead->category) == 0 ||
        strcmp("float", lookahead->category) == 0 ||
        strcmp("private", lookahead->category) == 0 ||
        strcmp("public", lookahead->category) == 0 ||
        strcmp("void", lookahead->category) == 0 ||
        strcmp("mult", lookahead->category) == 0 ||
        strcmp("div", lookahead->category) == 0 ||
        strcmp("and", lookahead->category) == 0 ||
        strcmp("plus", lookahead->category) == 0 ||
        strcmp("minus", lookahead->category) == 0 ||
        strcmp("or", lookahead->category) == 0 ||
        strcmp("eq", lookahead->category) == 0 ||
        strcmp("neq", lookahead->category) == 0 ||
        strcmp("geq", lookahead->category) == 0 ||
        strcmp("leq", lookahead->category) == 0 ||
        strcmp("lt", lookahead->category) == 0 ||
        strcmp("gt", lookahead->category) == 0) {

      create_leaf_node(get_nodeType_enum(lookahead->category), stack,
                       lookahead->lexeme, nodes_output, line);
      print_stack(stack, stack_contents);
    }

    if (lookahead->lexeme && strcmp(lookahead->lexeme, "eof"))
      free(lookahead->lexeme);
    if (lookahead)
      free(lookahead);

    lookahead = NULL;
    lookahead = malloc(sizeof(token));
    lookahead->lexeme = NULL;
    lookahead = get_next_token(lookahead, src, &line);

    while (!strcmp(lookahead->category, "inlinecmt") ||
           !strcmp(lookahead->category, "blockcmt")) {

      if (lookahead->lexeme && strcmp(lookahead->lexeme, "eof"))
        free(lookahead->lexeme);
      if (lookahead)
        free(lookahead);
      lookahead = NULL;
      lookahead = malloc(sizeof(token));
      if (!lookahead) {
        fprintf(stderr, "ERROR - match(): Could not allocate memory for the "
                        "lookahead exiting.\n");
        return -1;
      }
      lookahead->lexeme = NULL;
      lookahead = get_next_token(lookahead, src, &line);
    }
    return true;
  } else {
    char msg[128];
    snprintf(msg, sizeof(msg), "%s is the expected token.", type);
    syntax_error(msg);

    success = false;
    if (lookahead->lexeme && strcmp(lookahead->lexeme, "eof"))
      free(lookahead->lexeme);
    if (lookahead)
      free(lookahead);

    lookahead = NULL;
    lookahead = malloc(sizeof(token));
    lookahead->lexeme = NULL;
    lookahead = get_next_token(lookahead, src, &line);

    while (!strcmp(lookahead->category, "inlinecmt") ||
           !strcmp(lookahead->category, "blockcmt")) {

      if (lookahead->lexeme)
        free(lookahead->lexeme);
      if (lookahead)
        free(lookahead);

      lookahead = NULL;
      lookahead = malloc(sizeof(token));
      if (!lookahead) {
        fprintf(stderr, "ERROR - match(): Could not allocate memory for the "
                        "lookahead exiting.\n");
        return -1;
      }
      lookahead->lexeme = NULL;
      lookahead = get_next_token(lookahead, src, &line);
    }
    return true;
  }
}

// This function checks if a value is in the first set.
int in(const char *set[], int len) {
  if (!set) {
    error("ERROR - in(): set passed is null");
    return -1;
  }

  for (int i = 0; i < len; i++) {
    if (strcmp(lookahead->category, set[i]) == 0) {
      return true;
    }
  }
  return false;
}
// Note that we will have to add a EPSILON terminal symbol to each
// production that has an epsilon transition.
int contains_epsilon_transition(const char *first[], int first_len) {
  if (first == NULL) {
    error("contains_epsilon_transition() - FIRST set is NULL, terminating.");
    return -1;
  }

  for (int i = 0; i < first_len; i++) {
    if (strcasecmp("EPSILON", first[i]) == 0)
      return true;
  }
  return false;
}

int SkipErrors(const char *first[], const char *follow[], int first_len,
               int follow_len, const char *function) {
  // Valid continue to parse.
  if (in(first, first_len) || (contains_epsilon_transition(first, first_len) &&
                               in(follow, follow_len))) {
    return true;
  } else {

    char msg[1024];
    snprintf(msg, sizeof(msg), "%s error being processed in function %s ...",
             lookahead->category, function);
    syntax_error(msg);
    success = false;

    while (!in(first, first_len) && !in(follow, follow_len) &&
           strcasecmp(lookahead->category, "eof")) {

      if (lookahead->lexeme && strcmp(lookahead->lexeme, "eof"))
        free(lookahead->lexeme);
      if (lookahead)
        free(lookahead);

      lookahead = NULL;
      lookahead = malloc(sizeof(token));
      if (!lookahead) {
        fprintf(stderr, "ERROR - match(): Could not allocate memory for the "
                        "lookahead exiting.\n");
        return -1;
      }
      lookahead->lexeme = NULL;
      lookahead = get_next_token(lookahead, src, &line);

      if (contains_epsilon_transition(first, first_len) &&
          in(follow, follow_len)) {
        syntax_error(msg);
        success = false;
        return false;
      }
    }
    return true;
  }
}

// Functions below are all related to the parsing functions.
// This is the parse function  user should call returns -1 if there is problem 0
// is there are syntax errors and 1 if the parse
// was successful.

node *START();
/*
 * Because we use so much global state we do not need many parameters for this
 * function, would make calling it much more complicated.
 *
 *
 * This function needs to be refactored to accept all of the files and the
 * lookahead token, this may be a pain in the ass but it will make this code
 * much more modular.
 *
 * Note that we could potentially make this code much more modular by making the
 * functioncalls be parameterized but this would require a greate amout of
 * effort. At the moment it is not very complex, it is performing as it needs
 * and it is efficient the global state is being handled correctly, we only need
 * to be certain that we call free on the memory that we need to call free on.
 */
node *parse(const char *path) {
  int temp = initLookahead(path);
  if (temp == -1) {
    perror("ERROR - parse(): Cannot open file stream.");
    return NULL;
  } else if (temp == 0) {
    error("ERROR - parse(): Cannot allocate memory for the first token");
    return NULL;
  }

  char ast_out_file_name[1024];
  char nodes_out_file_name[1024];
  char stack_out_file_name[1024];

  char out_file_name[1024];
  char error_file_name[1024];
  char rule_file_name[1024];

  const char *file_name = extractFileName(path);

  // Now we will print to the correct directory.
  file_name = replaceSubstring(file_name, "tests", "output");

  snprintf(ast_out_file_name, sizeof(ast_out_file_name), "%s.outast",
           file_name);
  snprintf(nodes_out_file_name, sizeof(nodes_out_file_name), "%s.nodesout",
           file_name);
  snprintf(stack_out_file_name, sizeof(stack_out_file_name), "%s.stackcontent",
           file_name);
  snprintf(rule_file_name, sizeof(rule_file_name), "%s.rules", file_name);
  snprintf(out_file_name, sizeof(out_file_name), "%s.outderivation", file_name);
  snprintf(error_file_name, sizeof(error_file_name), "%s.outsyntaxerrors",
           file_name);

  printf("Printg rules to: %s \t Printing errors to: %s \n", out_file_name,
         error_file_name);

  error_output = fopen(error_file_name, "w+");
  derivation_output = fopen(out_file_name, "w+");
  rule_output = fopen(rule_file_name, "w+");
  stack_contents = fopen(stack_out_file_name, "w+");
  nodes_output = fopen(nodes_out_file_name, "w+");
  ast_output = fopen(ast_out_file_name, "w+");

  derivation_msg_stack = init_msg_stack();

  derivation_string = strdup("START");
  printf("DERIVATION FILE : %s \nERROR FILE: %s \n", out_file_name,
         error_file_name);
  if (!derivation_output)
    error("ERROR - parse() : cannot open the output file for derivation.");
  else
    printf("opened file for printing rules ...\n");
  if (!error_output)
    error("ERROR - parse(): Cannot open the file error for syntax errors.");
  else
    printf("opened file for printing errors ... \n");

  stack = init_stack();
  line = 1;

  node *root = NULL;
  success = true;

  if ((root = START()) != NULL) { // If the parse found a valid sentence ...
    printf("Parse complete valid program formed ... \n");

    print_tree_dot(root, ast_output);

    empty_stack(stack);
    if (stack->head)
      free(stack->head);
    if (stack)
      free(stack);
    stack = NULL;

    fclose(stack_contents);
    fclose(nodes_output);
    fclose(ast_output);
    ast_output = NULL;
    stack_contents = NULL;
    nodes_output = NULL;

    if (derivation_string)
      free(derivation_string);
    if (derivation_msg_stack)
      free(derivation_msg_stack);

    fclose(rule_output);
    rule_output = NULL;
    line = 1;
    success = true;
    if (lookahead)
      free(lookahead);
    lookahead = NULL;
    fclose(derivation_output);
    fclose(src);
    fclose(error_output);
    derivation_output = NULL;
    error_output = NULL;
    src = NULL;
    printf("Closing files ... \n");

    return root;
  } else { // If there were syntax errors we return false.
    printf("Invalid parse ... \n");

    empty_stack(stack);
    if (stack->head)
      free(stack->head);
    if (stack)
      free(stack);

    fclose(stack_contents);
    fclose(nodes_output);
    fclose(ast_output);

    ast_output = NULL;
    stack_contents = NULL;
    nodes_output = NULL;

    free(derivation_string);
    free(derivation_msg_stack);

    line = 1;
    success = true;
    if (lookahead)
      free(lookahead);

    fclose(rule_output);
    rule_output = NULL;
    lookahead = NULL;

    fclose(derivation_output);
    fclose(src);
    fclose(error_output);

    derivation_output = NULL;
    error_output = NULL;
    src = NULL;

    printf("Closing files ... \n");
    return NULL;
  }
}

// First and Follow sets for each non-terminal hardcoded.
const char *first_start[] = {"struct", "impl", "func", "eof"};
const char *follow_start[] = {};
const char *first_prog[] = {"struct", "impl", "func", "EPSILON"};
const char *follow_prog[] = {"eof"};
const char *first_reptprog0[] = {"struct", "impl", "func", "EPSILON"};
const char *follow_reptprog0[] = {"eof"};
const char *first_structorimplorfunc[] = {"struct", "impl", "func"};
const char *follow_structorimplorfunc[] = {"struct", "impl", "func", "eof"};
const char *first_structdecl[] = {"struct"};
const char *follow_structdecl[] = {"struct", "impl", "func", "eof"};
const char *first_optstructdecl2[] = {"inherits", "EPSILON"};
const char *follow_optstructdecl2[] = {"lcubr"};
const char *first_reptstructdecl4[] = {
    "public",
    "private",
    "EPSILON",
};
const char *follow_reptstructdecl4[] = {"rcubr"};
const char *first_visibility[] = {"private", "public"};
const char *follow_visibility[] = {"let", "func"};
const char *first_memberdecl[] = {"let", "func"};
const char *follow_memberdecl[] = {"rcubr", "public", "private"};
const char *first_funcdecl[] = {"func"};
const char *follow_funcdecl[] = {"rcubr", "public", "private"};
const char *first_vardecl[] = {"let"};
const char *follow_vardecl[] = {"public", "private", "let",    "id",   "while",
                                "read",   "write",   "return", "rcubr"};
const char *first_funchead[] = {"func"};
const char *follow_funchead[] = {"semi", "lcubr"};
const char *first_reptvardecl4[] = {"lsqbr", "EPSILON"};
const char *follow_reptvardecl4[] = {"semi"};
const char *first_fparams[] = {"id", "EPSILON"};
const char *follow_fparams[] = {"rpar"};

const char *first_reptfparams3[] = {"lsqbr", "EPSILON"};
const char *follow_reptfparams3[] = {"rpar", "comma"};
const char *first_returntype[] = {"integer", "float", "void", "id"};
const char *follow_returntype[] = {"semi", "lcubr"};

const char *first_type[] = {"integer", "float", "id"};
const char *follow_type[] = {"rpar", "lcubr", "comma", "lsqbr", "semi"};

const char *first_reptfparams4[] = {"comma", "EPSILON"};
const char *follow_reptfparams4[] = {"rpar"};

const char *first_reptfparamstail4[] = {"lsqbr", "EPSILON"};
const char *follow_reptfparamstail4[] = {"comma", "rpar"};
const char *first_fparamstail[] = {"comma"};
const char *follow_fparamstail[] = {"comma", "rpar"};
const char *first_funcdef[] = {"func"};
const char *follow_funcdef[] = {"struct", "impl", "func", "rcubr", "eof"};
const char *first_reptfuncbody1[] = {"let",  "id",    "if",     "while",
                                     "read", "write", "return", "EPSILON"};
const char *follow_reptfuncbody1[] = {"rcubr"};

const char *first_funcbody[] = {"lcubr"};
const char *follow_funcbody[] = {"struct", "impl", "func", "rcubr", "eof"};
const char *first_vardeclorstat[] = {"let",  "id",    "if",    "while",
                                     "read", "write", "return"};
const char *follow_vardeclorstat[] = {"let",  "id",    "if",     "while",
                                      "read", "write", "return", "rcubr"};
const char *first_statementbody[] = {"lpar", "lsqbr", "assign", "dot"};
const char *follow_statementbody[] = {"else",   "semi",  "let",  "id",
                                      "if",     "while", "read", "write",
                                      "return", "rcubr"};
const char *first_statement[] = {"id",   "if",    "while",
                                 "read", "write", "return"};
const char *follow_statement[] = {"else",  "semi", "let",   "id",     "if",
                                  "while", "read", "write", "return", "rcubr"};
const char *first_checkforendfunc[] = {"semi", "dot"};
const char *follow_checkforendfunc[] = {"else",   "semi",  "let",  "id",
                                        "if",     "while", "read", "write",
                                        "return", "rcubr"};
const char *first_checkforendvar[] = {"assign", "dot"};
const char *follow_checkforendvar[] = {"else",   "semi",  "if",   "let",
                                       "id",     "while", "read", "write",
                                       "return", "rcubr"};
const char *first_expr[] = {"id",  "intnum", "floatnum", "lpar",
                            "not", "plus",   "minus"};
const char *follow_expr[] = {"comma", "rpar", "semi"};
const char *first_arithexpr[] = {"id",  "intnum", "floatnum", "lpar",
                                 "not", "plus",   "minus"};
const char *follow_arithexpr[] = {"rsqbr", "eq",  "neq",   "lt",   "gt",
                                  "leq",   "geq", "comma", "rpar", "semi"};
const char *first_term[] = {"id",  "intnum", "floatnum", "lpar",
                            "not", "plus",   "minus"};
const char *follow_term[] = {"rsqbr", "eq",   "neq",  "lt",    "gt",
                             "leq",   "geq",  "plus", "minus", "or",
                             "comma", "rpar", "semi"};

const char *first_factor[] = {"id",  "intnum", "floatnum", "lpar",
                              "not", "plus",   "minus"};
const char *follow_factor[] = {
    "mult", "div", "and",  "rsqbr", "eq", "neq",   "lt",   "gt",
    "leq",  "geq", "plus", "minus", "or", "comma", "rpar", "semi"};
const char *first_varfuncstatenest2[] = {"lpar", "lsqbr", "assign", "dot"};
const char *follow_varfuncstatenest2[] = {"else",   "semi",  "let",  "id",
                                          "if",     "while", "read", "write",
                                          "return", "rcubr"};
const char *first_varfuncstatenest[] = {"dot"};
const char *follow_varfuncstatenest[] = {"else",   "semi",  "let",  "id",
                                         "if",     "while", "read", "write",
                                         "return", "rcubr"};
const char *first_addop[] = {"plus", "minus", "or"};
const char *follow_addop[] = {"intnum", "floatnum", "id",   "lpar",
                              "not",    "plus",     "minus"};
const char *first_indice[] = {"lsqbr"};
const char *follow_indice[] = {"assign", "mult",  "div",  "and",  "lsqbr",
                               "dot",    "rsqbr", "eq",   "neq",  "lt",
                               "gt",     "leq",   "geq",  "plus", "minus",
                               "or",     "comma", "rpar", "semi"};
const char *first_arraysize[] = {"lsqbr"};
const char *follow_arraysize[] = {"semi", "lsqbr", "rpar", "comma"};
const char *first_reptoptstructdecl22[] = {"comma", "EPSILON"};
const char *follow_reptoptstructdecl22[] = {"lcubr"};
const char *first_reptvariable[] = {"dot", "EPSILON"};
const char *follow_reptvariable[] = {"rpar"};
const char *first_varidnest[] = {"dot"};
const char *follow_varidnest[] = {"dot", "rpar", "lpar", "lsqbr"};
const char *first_varidnest2[] = {"lpar", "lsqbr", "EPSILON"};
const char *follow_varidnest2[] = {"dot", "rpar"};
const char *first_rightrecarithexpr[] = {"plus", "minus", "or", "EPSILON"};
const char *follow_rightrecarithexpr[] = {
    "rsqbr", "eq", "neq", "lt", "gt", "leq", "geq", "comma", "rpar", "semi"};
const char *first_variable2[] = {"lpar", "lsqbr", "dot", "EPSILON"};
const char *follow_variable2[] = {"rpar"};
const char *first_relexpr[] = {"id",  "intnum", "floatnum", "lpar",
                               "not", "plus",   "minus"};
const char *follow_relexpr[] = {"rpar"};
const char *first_reptstatblock1[] = {"id",    "if",     "while",  "read",
                                      "write", "return", "EPSILON"};
const char *follow_reptstatblock1[] = {"rcubr"};

const char *first_reptimpldef3[] = {"func", "EPSILON"};
const char *follow_reptimpldef3[] = {"rcubr"};

const char *first_impldef[] = {"impl"};
const char *follow_impldef[] = {"struct", "impl", "func", "eof"};
const char *first_statblock[] = {"lcubr", "id",    "if",     "while",
                                 "read",  "write", "return", "EPSILON"};
const char *follow_statblock[] = {"else", "semi"};
const char *first_aparamstail[] = {"comma"};
const char *follow_aparamstail[] = {"comma", "rpar"};
const char *first_rightrecterm[] = {"mult", "div", "and", "EPSILON"};
const char *follow_rightrecterm[] = {"rsqbr", "eq",   "neq",  "lt",    "gt",
                                     "leq",   "geq",  "plus", "minus", "or",
                                     "comma", "rpar", "semi"};
const char *first_relop[] = {"eq", "neq", "lt", "gt", "leq", "geq"};
const char *follow_relop[] = {"intnum", "floatnum", "lpar",
                              "not",    "plus",     "minus"};
const char *first_flsqbr[] = {"intnum", "rsqbr"};
const char *follow_flsqbr[] = {"semi", "lsqbr", "rpar", "comma"};

const char *first_reptaparams1[] = {"comma", "EPSILON"};
const char *follow_reptaparams1[] = {"rpar"};

const char *first_sign[] = {"plus", "minus"};
const char *follow_sign[] = {"id",  "intnum", "floatnum", "lpar",
                             "not", "plus",   "minus"};
const char *first_variable[] = {"id"};
const char *follow_variable[] = {"rpar"};
const char *first_multop[] = {"mult", "div", "and"};
const char *follow_multop[] = {"id",  "intnum", "flotnum", "lpar",
                               "not", "plus",   "minus"};

const char *first_assignop[] = {"assign"};
const char *follow_assignop[] = {"id",  "floatnum", "intnum", "lpar",
                                 "not", "plus",     "minus"};
const char *first_aparams[] = {"id",  "intnum", "floatnum", "lpar",
                               "not", "plus",   "minus",    "EPSILON"};
const char *follow_aparams[] = {"rpar"};
const char *first_idnest2[] = {"EPSILON", "lpar", "lsqbr"};
const char *follow_idnest2[] = {"mult", "div",  "and",    "dot", "rsqbr",
                                "eq",   "neq",  "lt",     "gt",  "leq",
                                "geq",  "plus", "minus",  "or",  "comma",
                                "rpar", "semi", "EPSILON"};
const char *first_idnest[] = {"dot"};
const char *follow_idnest[] = {"mult",  "div", "and",   "dot",  "rsqbr", "eq",
                               "neq",   "lt",  "gt",    "leq",  "geq",   "plus",
                               "minus", "or",  "comma", "rpar", "semi"};
const char *first_reptvariableorfunctioncall[] = {"dot", "EPSILON"};
const char *follow_reptvariableorfunctioncall[] = {
    "mult", "div", "and",  "rsqbr", "eq", "neq",   "lt",   "gt",
    "leq",  "geq", "plus", "minus", "or", "comma", "rpar", "semi"};
const char *first_reptidnest1[] = {"lsqbr", "EPSILON"};
const char *follow_reptidnest1[] = {
    "assign", "mult", "div", "and",  "dot",   "rsqbr", "eq",    "neq",  "lt",
    "gt",     "leq",  "geq", "plus", "minus", "or",    "comma", "rpar", "semi"};
const char *first_factor2[] = {"lpar", "lsqbr", "EPSILON"};
const char *follow_factor2[] = {
    "mult", "div", "and",  "dot",   "rsqbr", "eq",    "neq",  "lt",  "gt",
    "leq",  "geq", "plus", "minus", "or",    "comma", "rpar", "semi"};
const char *first_optrelexpr[] = {"eq",  "neq", "lt",     "gt",
                                  "leq", "geq", "EPSILON"};
const char *follow_optrelexpr[] = {"comma", "rpar", "semi"};

int PROG();

// START -> PROG eof .
node *START() {
  if (!SkipErrors(first_start, follow_start, 4, 0, "START"))
    return false;

  if (in(first_prog, 4)) {

    node *prognode = init_node(prog);
    push_node(prognode, stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "START", "PROG eof");
    if (PROG() & match("eof")) {

      create_prog_tree(stack, nodes_output);
      node *temp = create_subtree(prog, stack, nodes_output);

      return temp;
      print_rule("START -> PROG eof");
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

int REPTPROG0();

/*
PROG -> REPTPROG0 .
 */
int PROG() {

  if (!SkipErrors(first_prog, follow_prog, 4, 1, "PROG"))
    return false;
  if (in(first_reptprog0, 3)) {
    updateDerivation(&derivation_string, "PROG", "REPTPROG0");

    if (REPTPROG0())
      print_rule("PROG -> REPTPROG0");
    else
      success = false;
  } else {
    success = false;
  }

  return success;
}

int STRUCTORIMPLORFUNC();
int REPTPROG0() {

  if (!SkipErrors(first_reptprog0, follow_reptprog0, 4, 1, "REPTPROG0"))
    return false;

  if (in(first_structorimplorfunc, 3)) {
    updateDerivation(&derivation_string, "REPTPROG0",
                     "STRUCTORIMPLORFUNC REPTPROG0");

    if (STRUCTORIMPLORFUNC() & REPTPROG0()) {
      print_rule("REPTPROG0 -> STRUCTORIMPLORFUNC REPTPROG0");
    } else {
      success = false;
    }
  } else if (in(follow_reptprog0, 1)) {
    updateDerivation(&derivation_string, "REPTPROG0", "");
    print_rule("REPTPROG0 -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

int ADDOP() {

  if (!SkipErrors(first_addop, follow_addop, 3, 7, "ADDOP"))
    return false;
  if (compare("plus")) {
    updateDerivation(&derivation_string, "ADDOP", "plus");
    if (match("plus"))
      print_rule("ADDOP -> plus");
    else
      success = false;
  } else if (compare("minus")) {
    updateDerivation(&derivation_string, "ADDOP", "minus");
    if (match("minus"))
      print_rule("ADDOP -> minus");
    else
      success = false;
  } else if (compare("or")) {
    updateDerivation(&derivation_string, "ADDOP", "or");
    if (match("or"))
      print_rule("ADDOP -> or");
    else
      success = false;
  } else
    success = false;

  return success;
}

int EXPR();
int REPT_APARAMS1();
/*
APARAMS -> EXPR REPTAPARAMS1
          |.
 */
int APARAMS() {

  if (!SkipErrors(first_aparams, follow_aparams, 8, 1, "APARAMS"))
    return false;
  if (in(first_expr, 7)) {

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "APARAMS", "EXPR REPT_APARAMS1");
    if (EXPR() & REPT_APARAMS1()) {

      node *aparamslistnode =
          create_subtree_pop_until(aparamslist, sentinel, stack, line);
      print_stack(stack, stack_contents);
      push_node(aparamslistnode, stack);
      print_stack(stack, stack_contents);

      print_rule("APARAMS -> EXPR REPT_APARAMS1");
    } else {
      success = false;
    }
  } else if (in(follow_aparams, 1)) {
    push_node(init_node(aparamslist), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "APARAMS", "");
    print_rule("APARAMS -> EPSILON");
  }

  return success;
}

/*
APARAMSTAIL -> comma EXPR .
 */
int APARAMS_TAIL() {

  if (!SkipErrors(first_aparamstail, follow_aparamstail, 1, 2, "APARAMS_TAIL"))
    return false;

  if (in(first_aparamstail, 1)) {
    updateDerivation(&derivation_string, "APARAMS_TAIL", "comma EXPR");
    if (match("comma") & EXPR())
      print_rule("APARAMS_TAIL -> comma EXPR");
    else
      success = false;
  } else {
    success = false;
  }

  return success;
}

int TERM();
int RIGHTRECARITHEXPR();
/*
ARITHEXPR -> TERM RIGHTRECARITHEXPR .

not nullable, cannot have an empty indice
 */
int ARITHEXPR() {

  if (!SkipErrors(first_arithexpr, follow_arithexpr, 7, 10, "ARITHEXPR"))
    return false;
  if (in(first_term, 7)) {
    updateDerivation(&derivation_string, "ARITHEXPR", "TERM RIGHTRECARITHEXPR");
    if (TERM() & RIGHTRECARITHEXPR())
      print_rule("ARITHEXPR -> TERM RIGHTRECARITHEXPR");
    else
      success = false;
  } else {
    success = false;
  }

  return success;
}

int FLSQBR();

/*
ARRAYSIZE -> lsqbr Flsqbr .
 */
int ARRAYSIZE() {

  if (!SkipErrors(first_arraysize, follow_arraysize, 1, 4, "ARRAYSIZE"))
    return false;
  if (in(first_arraysize, 1)) {
    updateDerivation(&derivation_string, "ARRAYSIZE", "lsqbr FLSQBR");
    if (match("lsqbr") & FLSQBR())
      print_rule("ARRAYSIZE -> lsqbr FLSQBR");
    else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
Flsqbr -> intnum rsqbr
         |rsqbr .
 */
int FLSQBR() {

  if (!SkipErrors(first_flsqbr, follow_flsqbr, 2, 4, "Flsqbr"))
    return false;
  if (compare("intnum")) {
    updateDerivation(&derivation_string, "FLSQBR", "intnum rsqbr");
    if (match("intnum") & match("rsqbr"))
      print_rule("FLSQBR -> intnum rsqbr");
    else
      success = false;
  } else if (compare("rsqbr")) {
    updateDerivation(&derivation_string, "FLSQBR", "rsqbr");
    if (match("rsqbr")) {

      node *temp = init_node(emptydim);
      temp->value = "nil";
      push_node(temp, stack);
      print_stack(stack, stack_contents);

      print_rule("FLSQBR -> rsqbr");
    }
  } else {
    success = false;
  }

  return success;
}

/*
ASSIGNOP -> equal .
 */
int ASSIGNOP() {

  if (!SkipErrors(first_assignop, follow_assignop, 1, 7, "ASSIGNOP"))
    return false;
  if (in(first_assignop, 1)) {
    updateDerivation(&derivation_string, "ASSIGNOP", "assign");
    if (match("assign"))
      print_rule("ASSIGNOP -> assign");
    else
      success = false;
  } else
    success = false;

  return success;
}

/*
EXPR -> ARITHEXPR OPTRELEXPR .
 */
int OPTRELEXPR();
int EXPR() {

  if (!SkipErrors(first_expr, follow_expr, 7, 3, "EXPR"))
    return false;
  if (in(first_arithexpr, 7)) {
    updateDerivation(&derivation_string, "EXPR", "ARITHEXPR OPTRELEXPR");

    if (ARITHEXPR() & OPTRELEXPR()) {
      print_rule("EXPR -> ARITHEXPR OPTRELEXPR");
    } else {
      success = false;
    }
  } else {
    success = false;
  }

  return success;
}

int RELOP();
/*
OPTRELEXPR -> RELOP ARITHEXPR | .
 */
int OPTRELEXPR() {

  if (!SkipErrors(first_optrelexpr, follow_optrelexpr, 7, 3, "OPTRELEXPR"))
    return false;
  if (in(first_relop, 6)) {
    updateDerivation(&derivation_string, "OPTRELEXPR", "RELOP ARITHEXPR");
    if (RELOP() & ARITHEXPR()) {

      node *relexprnode = create_subtree_n_nodes(relexpr, 3, stack, line);
      push_node(relexprnode, stack);
      print_stack(stack, stack_contents);

      print_rule("OPTRELEXPR -> RELOP ARITHEXPR");
    } else {
      success = false;
    }
  } else if (in(follow_optrelexpr, 3)) {
    updateDerivation(&derivation_string, "OPTRELEXPR", "");
    print_rule("OPTRELEXPR -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

int FACTOR2();
int REPTVARIABLEORFUNCTIONCALL();

/*
FACTOR -> id FACTOR2 REPTVARIABLEORFUNCTIONCALL
         |intlit
         |floatlit
         |lpar ARITHEXPR rpar
         |not FACTOR
         |SIGN FACTOR .
  */
int SIGN();
int FACTOR() {

  if (!SkipErrors(first_factor, follow_factor, 7, 16, "FACTOR"))
    return false;
  if (compare("id")) {
    updateDerivation(&derivation_string, "FACTOR",
                     "id FACTOR2 REPTVARIABLEORFUNCTIONCALL");
    if (match("id") & FACTOR2() & REPTVARIABLEORFUNCTIONCALL())
      print_rule("FACTOR -> FACTOR2 REPTVARIABLEORFUNCTIONCALL");
    else
      success = false;
  } else if (compare("intnum")) {
    updateDerivation(&derivation_string, "FACTOR", "intlit");
    if (match("intnum"))
      print_rule("FACTOR -> intlit");
    else
      success = false;
  } else if (compare("floatnum")) {
    updateDerivation(&derivation_string, "FACTOR", "floatlit");
    if (match("floatnum"))
      print_rule("FACTOR -> floatlit");
    else
      success = false;
  } else if (compare("not")) {
    updateDerivation(&derivation_string, "FACTOR", "not FACTOR");
    if (match("not") & FACTOR()) {

      push_node(create_subtree_n_nodes(notnode, 1, stack, line), stack);
      print_stack(stack, stack_contents);
      print_rule("FACTOR -> not FACTOR");
    } else
      success = false;
  } else if (compare("lpar")) {
    updateDerivation(&derivation_string, "FACTOR", "lpar ARITHEXPR rpar");
    if (match("lpar") && ARITHEXPR() && match("rpar"))
      print_rule("FACTOR -> lpar ARITHEXPR rpar");
    else
      success = false;
  } else if (in(first_sign, 2)) {
    updateDerivation(&derivation_string, "FACTOR", "SIGN FACTOR");
    if (SIGN() & FACTOR()) {

      push_node(create_subtree_n_nodes(sign, 2, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("FACTOR -> SIGN FACTOR");
    } else {
      success = false;
    }
  } else {
    success = false;
  }

  return success;
}

// FACTOR2 -> lpar APARAMS rpar | REPTIDNEST1  .

int REPTIDNEST1();

int FACTOR2() {

  if (!SkipErrors(first_factor2, follow_factor2, 3, 17, "FACTOR2"))
    return false;

  if (compare("lpar")) {

    updateDerivation(&derivation_string, "FACTOR2", "lpar APARAMS rpar");
    if (match("lpar") & APARAMS() & match("rpar")) {

      node *funccallnode = create_subtree_n_nodes(funccall, 2, stack, line);
      print_stack(stack, stack_contents);
      push_node(funccallnode, stack);

    } else {
      success = false;
    }
  } else if (in(first_reptidnest1, 2)) {

    node *epsilon = init_node(sentinel);
    push_node(epsilon, stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "FACTOR2", "REPTIDNEST1");
    if (REPTIDNEST1()) {

      push_node(create_subtree_pop_until(dimlist, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      node *varnode = create_subtree_n_nodes(var, 2, stack, line);
      push_node(varnode, stack);
      print_stack(stack, stack_contents);

      print_rule("FACTOR2 -> REPTIDNEST1");
    }
  } else if (in(follow_factor2, 17)) {

    push_node(init_node(dimlist), stack);
    print_stack(stack, stack_contents);

    push_node(create_subtree_n_nodes(var, 2, stack, line), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "FACTOR2", "");
    print_rule("FACTOR2 -> EPSILON");
  }

  else {
    success = false;
  }

  return success;
}

/*
FPARAMS -> id colon TYPE REPTFPARAMS3 REPTFPARAMS4
          |.
  */

int TYPE();
int REPTFPARAMS3();
int REPTFPARAMS4();
int FPARAMS() {

  if (!SkipErrors(first_fparams, follow_fparams, 2, 1, "FPARAMS"))
    return false;
  if (compare("id")) {

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "FPARAMS",
                     "id colon TYPE REPTFPARAMS3 REPTFPARAMS4");

    if (match("id") & match("colon") & TYPE()) {

      push_node(init_node(sentinel), stack);
      print_stack(stack, stack_contents);

      if (REPTFPARAMS3()) {

        push_node(create_subtree_pop_until(arraydims, sentinel, stack, line),
                  stack);
        print_stack(stack, stack_contents);
        push_node(create_subtree_n_nodes(fparam, 3, stack, line), stack);
        print_stack(stack, stack_contents);

        if (REPTFPARAMS4()) {

          push_node(
              create_subtree_pop_until(fparamslist, sentinel, stack, line),
              stack);
          print_stack(stack, stack_contents);

          print_rule("FPARAMS -> id colon TYPE REPTFPARAMS3 REPTFPARAMS4");
        }
      } else
        success = false;
    } else
      success = false;
  } else if (in(follow_fparams, 1)) {

    push_node(init_node(fparamslist), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "FPARAMS", "");
    print_rule("FPARAMS -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

// FPARAMSTAIL -> comma id colon TYPE REPTFPARAMSTAIL4 . //
int REPTFPARAMSTAIL4();

int FPARAMSTAIL() {

  if (!SkipErrors(first_fparamstail, follow_fparamstail, 1, 2, "FPARAMSTAIL"))
    return false;

  if (compare("comma")) {

    updateDerivation(&derivation_string, "FPARAMSTAIL",
                     "comma id colon TYPE REPTFPARAMSTAIL4");
    if (match("comma") & match("id") & match("colon") & TYPE()) {

      push_node(init_node(sentinel), stack);
      print_stack(stack, stack_contents);

      if (REPTFPARAMSTAIL4()) {

        push_node(create_subtree_pop_until(arraydims, sentinel, stack, line),
                  stack);
        print_stack(stack, stack_contents);

        print_rule("FPARAMSTAIL -> comma id colon TYPE REPTFPARAMSTAIL4");
      } else {
        success = false;
      }
    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}

// FUNCBODY -> lcubr REPTFUNCBODY1 rcubr . //

int REPTFUNCBODY1();
int FUNCBODY() {

  if (!SkipErrors(first_funcbody, follow_funcbody, 1, 5, "FUNCBODY"))
    return false;

  if (compare("lcubr")) {
    updateDerivation(&derivation_string, "FUNCBODY",
                     "lcubr REPTFUNCBODY1 rcubr");

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    if (match("lcubr") & REPTFUNCBODY1() & match("rcubr")) {

      push_node(create_subtree_pop_until(funcbody, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      print_stack(stack, stack_contents);

      print_rule("FUNCBODY -> lcubr REPTFUNCBODY1 rcubr");
    } else {
      success = false;
    }
  } else {
    success = false;
  }

  return success;
}

// FUNCDECL -> FUNCHEAD semi .
int FUNCHEAD();
int FUNCDECL() {

  if (!SkipErrors(first_funcdecl, follow_funcdecl, 1, 3, "FUNCDECL"))
    return false;
  if (in(first_funchead, 1)) {
    updateDerivation(&derivation_string, "FUNCDECL", "FUNCHEAD semi");
    if (FUNCHEAD() & match("semi"))
      print_rule("FUNCDECL -> FUNCHEAD semi");
    else
      success = false;
  } else {
    success = false;
  }
  return success;
}

// FUNCDEF -> FUNCHEAD FUNCBODY . //

int FUNCDEF() {

  if (!SkipErrors(first_funcdef, follow_funcdef, 1, 5, "FUNCDEF"))
    return false;

  if (in(first_funchead, 1)) {
    updateDerivation(&derivation_string, "FUNCDEF", "FUNCHEAD FUNCBODY");

    if (FUNCHEAD() & FUNCBODY()) {

      push_node(create_subtree_n_nodes(funcdef, 4, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("FUNCDEF -> FUNCHEAD FUNCBODY");
    } else
      success = false;
  } else {
    success = false;
  }
  return success;
}

// FUNCHEAD -> func id lpar FPARAMS rpar arrow RETURNTYPE . //

int RETURNTYPE();

int FUNCHEAD() {

  if (!SkipErrors(first_funchead, follow_funchead, 1, 2, "FUNCHEAD"))
    return false;

  if (compare("func")) {
    updateDerivation(&derivation_string, "FUNCHEAD",
                     "func id lpar FPARAMS rpar arrow RETURNTYPE");

    if (match("func") & match("id") & match("lpar") & FPARAMS() &
        match("rpar") & match("arrow") & RETURNTYPE()) {

      print_rule("FUNCHEAD -> func id lpar FPARAMS rpar arrow RETURNTYPE");
    } else
      success = false;
  } else
    success = false;

  return success;
}

// REPTVARIABLEORFUNCTIONCALL -> IDNEST REPTVARIABLEORFUNCTIONCALL | . //

int IDNEST();

int REPTVARIABLEORFUNCTIONCALL() {

  if (!SkipErrors(first_reptvariableorfunctioncall,
                  follow_reptvariableorfunctioncall, 2, 16,
                  "REPTVARIABLEORFUNCTIONCALL"))
    return false;
  if (in(first_idnest, 1)) {
    updateDerivation(&derivation_string, "REPTVARIABLEORFUNCTIONCALL",
                     "IDNEST REPTVARIABLEORFUNCTIONCALL");
    if (IDNEST() & REPTVARIABLEORFUNCTIONCALL()) {

      push_node(create_subtree_n_nodes(dot, 2, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule(
          "REPTVARIABLEORFUNCTIONCALL -> IDNEST REPTVARIABLEORFUNCTIONCALL");
    } else
      success = false;
  } else if (in(follow_reptvariableorfunctioncall, 16)) {
    updateDerivation(&derivation_string, "REPTVARIABLEORFUNCTIONCALL", "");
    print_rule("REPTVARIABLEORFUNCTIONCALL -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

// IDNEST -> dot id IDNEST2 . //

int IDNEST2();

int IDNEST() {

  if (!SkipErrors(first_idnest, follow_idnest, 1, 17, "IDNEST"))
    return false;

  if (compare("dot")) {
    updateDerivation(&derivation_string, "IDNEST", "dot id IDNEST2");
    if (match("dot") & match("id") & IDNEST2())
      print_rule("IDNEST -> dot id IDNEST2");
    else
      success = false;
  } else {
    success = false;
  }
  return success;
}

// IDNEST2 ->lpar APARAMS rpar | REPTIDNEST1 . //
int IDNEST2() {
  if (!SkipErrors(first_idnest2, follow_idnest2, 3, 18, "IDNEST2"))
    return false;

  if (compare("lpar")) {

    updateDerivation(&derivation_string, "IDNEST2", "lpar APARAMS rpar");
    if (match("lpar") & APARAMS() & match("rpar")) {

      push_node(create_subtree_n_nodes(funccall, 2, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("IDNEST2 -> lpar APARAMS rpar");
    } else
      success = false;
  } else if (in(first_reptidnest1, 2)) {

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "IDNEST2", "REPTIDNEST1");
    if (REPTIDNEST1()) {

      push_node(create_subtree_pop_until(dimlist, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      push_node(create_subtree_n_nodes(var, 2, stack, line), stack);
      print_stack(stack, stack_contents);
      print_rule("IDNEST2 -> REPTIDNEST1");
    } else
      success = false;
  } else if (in(follow_idnest2, 17)) {

    push_node(init_node(dimlist), stack);
    print_stack(stack, stack_contents);
    push_node(create_subtree_n_nodes(var, 2, stack, line), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "IDNEST2", "");
    print_rule("IDNEST2 -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

// IMPLDEF -> impl id lcubr REPTIMPLDEF3 rcubr .

int REPTIMPLDEF3();

int IMPLDEF() {

  if (!SkipErrors(first_impldef, follow_impldef, 1, 12, "IMPLDEF"))
    return false;
  if (compare("impl")) {

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "IMPLDEF",
                     "impl id lcubr REPTIMPLDEF3 rcubr");
    if (match("impl") & match("id") & match("lcubr") & REPTIMPLDEF3() &
        match("rcubr")) {

      push_node(create_subtree_pop_until(impldef, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      print_rule("IMPLDEF -> impl id lcubr REPTIMPLDEF3 rcubr");
    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}

// INDICE -> lsqbr ARITHEXPR rsqbr .

int INDICE() {
  if (!SkipErrors(first_indice, follow_indice, 1, 19, "INDICE"))
    return false;
  if (compare("lsqbr")) {
    updateDerivation(&derivation_string, "INDICE", "rsqbr ARITHEXPR lsqbr");
    if (match("lsqbr") & ARITHEXPR() & match("rsqbr"))
      print_rule("lsqbr ARITHEXPR rsqbr");
    else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
MEMBERDECL -> FUNCDECL
             |VARDECL .
 */
int VARDECL();
int MEMBERDECL() {

  if (!SkipErrors(first_memberdecl, follow_memberdecl, 2, 3, "MEMBERDECL"))
    return false;
  if (in(first_funcdecl, 1)) {

    updateDerivation(&derivation_string, "MEMBERDECL", "FUNCDECL");

    if (FUNCDECL()) {

      push_node(create_subtree_n_nodes(funcdecl, 4, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("MEMBERDECL -> FUNCDECL");
    } else
      success = false;
  } else if (in(first_vardecl, 1)) {

    updateDerivation(&derivation_string, "MEMBERDECL", "VARDECL");
    if (VARDECL()) {

      push_node(create_subtree_n_nodes(vardecl, 4, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("MEMBERDECL -> VARDECL");

    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
MULTOP -> mult
         |div
         |and .
 */

int MULTOP() {

  if (!SkipErrors(first_multop, follow_multop, 3, 7, "MULTOP"))
    return false;
  if (compare("mult")) {
    updateDerivation(&derivation_string, "MULTOP", "mult");
    if (match("mult"))
      print_rule("MULTOP -> mult");
    else
      success = false;
  } else if (compare("div")) {
    updateDerivation(&derivation_string, "MULTOP", "div");
    if (match("div"))
      print_rule("MULTOP -> div");
    else
      success = false;
  } else if (compare("and")) {
    updateDerivation(&derivation_string, "MULTOP", "and");
    if (match("and"))
      print_rule("MULTOP -> and");
    else
      success = false;
    ;
  } else {
    success = false;
  }
  return success;
}

/*
OPTSTRUCTDECL2 -> inherits id REPTOPTSTRUCTDECL22
                 |.
 */

int REPTOPTSTRUCTDECL22();

int OPTSTRUCTDECL2() {

  if (!SkipErrors(first_optstructdecl2, follow_optstructdecl2, 2, 1,
                  "OPTSTRUCTDECL2"))
    return false;
  if (compare("inherits")) {

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "OPTSTRUCTDECL2",
                     "inherits id REPTOPTSTRUCTDECL22");
    if (match("inherits") & match("id") & REPTOPTSTRUCTDECL22()) {

      push_node(create_subtree_pop_until(inherits, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      print_rule("OPTSTRUCTDECL2 -> inherits id REPTOPTSTRUCTDECL22");
    } else
      success = false;
  } else if (in(follow_optstructdecl2, 1)) {

    updateDerivation(&derivation_string, "OPTSTRUCTDECL2", "");
    print_rule("OPTSTRUCTDECL2 -> EPSILON");

  } else {
    success = false;
  }

  return success;
}

// RELEXPR -> ARITHEXPR RELOP ARITHEXPR .//

int RELEXPR() {

  if (!SkipErrors(first_relexpr, follow_relexpr, 7, 1, "RELEXPR"))
    return false;
  if (in(first_arithexpr, 7)) {
    updateDerivation(&derivation_string, "RELEXPR",
                     "ARITHEXPR RELOP ARITHEXPR");
    if (ARITHEXPR() & RELOP() & ARITHEXPR()) {

      push_node(create_subtree_n_nodes(relexpr, 3, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("RELEXPR -> ARITHEXPR RELOP ARITHEXPR");
    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
RELOP -> eq
        |neq
        |lt
        |gt
        |leq
        |geq .
 */

int RELOP() {

  if (!SkipErrors(first_relop, follow_relop, 6, 6, "RELOP"))
    return false;
  if (compare("eq")) {

    updateDerivation(&derivation_string, "RELOP", "eq");
    if (match("eq")) {
      print_rule("RELOP -> eq");
    } else
      success = false;
  } else if (compare("neq")) {
    updateDerivation(&derivation_string, "RELOP", "neq");
    if (match("neq")) {
      print_rule("RELOP -> neq");
    } else
      success = false;
  } else if (compare("lt")) {
    updateDerivation(&derivation_string, "RELOP", "lt");
    if (match("lt")) {

      print_rule("RELOP -> lt");

    } else
      success = false;
  } else if (compare("gt")) {
    updateDerivation(&derivation_string, "RELOP", "gt");
    if (match("gt")) {

      print_rule("RELOP -> gt ");
    } else
      success = false;
  } else if (compare("leq")) {
    updateDerivation(&derivation_string, "RELOP", "leq");
    if (match("leq")) {
      print_rule("RELOP -> leq");
    } else
      success = false;
  } else if (compare("geq")) {
    updateDerivation(&derivation_string, "RELOP", "geq");
    if (match("geq")) {

      print_rule("RELOP -> geq");
    } else
      success = false;
  } else
    success = false;

  return success;
}

/*
REPT_APARAMS1 -> APARAMS_TAIL REPT_APARAMS1 //
               |.
 */

int REPT_APARAMS1() {

  if (!SkipErrors(first_reptaparams1, follow_reptaparams1, 2, 1,
                  "REPT_APARAMS1"))
    return false;
  if (in(first_aparamstail, 2)) {
    updateDerivation(&derivation_string, "REPT_APARAMS1",
                     "APARAMS_TAIL REPT_APARAMS1");
    if (APARAMS_TAIL() & REPT_APARAMS1())
      print_rule("REPT_APARAMS1 -> APARAMS_TAIL REPT_APARAMS1");
    else
      success = false;
    ;
  } else if (in(follow_reptaparams1, 1)) {
    updateDerivation(&derivation_string, "REPT_APARAMS1", "");
    print_rule("REPT_APARAMS1 -> EPSILON");
  } else
    success = false;

  return success;
}

/*
REPTFPARAMS3 -> ARRAYSIZE REPTFPARAMS3
               |.
 */

int REPTFPARAMS3() {

  if (!SkipErrors(first_reptfparams3, follow_reptfparams3, 2, 2,
                  "REPTFPARAMS3"))
    return false;

  if (in(first_arraysize, 1)) {

    updateDerivation(&derivation_string, "REPTFPARAMS3",
                     "ARRAYSIZE REPTFPARAMS3");

    if (ARRAYSIZE() & REPTFPARAMS3())
      print_rule("REPTFPARAMS3 -> ARRAYSIZE REPTFPARAMS3");
    else
      success = false;
  } else if (in(follow_reptfparams3, 2)) {

    updateDerivation(&derivation_string, "REPTFPARAMS3", "");
    print_rule("REPTFPARAMS3 -> EPSILON");
  } else
    success = false;

  return success;
}

/*
REPTFPARAMS4 -> FPARAMSTAIL REPTFPARAMS4
               |.
 */

int REPTFPARAMS4() {

  if (!SkipErrors(first_reptfparams4, follow_reptfparams4, 2, 1,
                  "REPTFPARAMS4"))
    return false;
  if (in(first_fparamstail, 1)) {

    updateDerivation(&derivation_string, "REPTFPARAMS4",
                     "FPARAMSTAIL REPTFPARAMS4");
    if (FPARAMSTAIL()) {

      push_node(create_subtree_n_nodes(fparam, 3, stack, line), stack);
      print_stack(stack, stack_contents);

      if (REPTFPARAMS4())
        print_rule("REPTFPARAMS -> FPARAMSTAIL REPTFPARAMS4");
      else
        success = false;
    } else
      success = false;
  } else if (in(follow_reptfparams4, 1)) {

    updateDerivation(&derivation_string, "REPTFPARAMS4", "");
    print_rule("REPTFPARAMS4 -> EPSILON");
  } else
    success = false;

  return success;
}

/*
REPTFPARAMSTAIL4 -> ARRAYSIZE REPTFPARAMSTAIL4
                   |.
 */

int REPTFPARAMSTAIL4() {

  if (!SkipErrors(first_reptfparamstail4, follow_reptfparamstail4, 2, 2,
                  "REPTFPARAMSTAIL4"))
    return false;
  if (in(first_arraysize, 1)) {
    updateDerivation(&derivation_string, "REPTFPARAMSTAIL4",
                     "ARRAYSIZE REPTFPARAMSTAIL4");
    if (ARRAYSIZE() & REPTFPARAMSTAIL4())
      print_rule("REPTFPARAMSTAIL4 -> ARRAYSIZE REPTFPARAMSTAIL4");
    else
      success = false;
  } else if (in(follow_reptfparamstail4, 2)) {

    updateDerivation(&derivation_string, "REPTFPARAMSTAIL4", "");
    print_rule("REPTFPARAMSTAIL4 -> EPSILON");
  } else
    success = false;

  return success;
}

/*
REPTFUNCBODY1 -> VARDECLORSTAT REPTFUNCBODY1 //
                |.
 */
int VARDECLORSTAT();
int REPTFUNCBODY1() {

  if (!SkipErrors(first_reptfuncbody1, follow_reptfuncbody1, 8, 1,
                  "REPTFUNCBODY1"))
    return false;
  if (in(first_vardeclorstat, 7)) {
    updateDerivation(&derivation_string, "REPTFUNCBODY1",
                     "VARDECLORSTAT REPTFUNCBODY1");
    if (VARDECLORSTAT() & REPTFUNCBODY1())
      print_rule("REPTFUNCBODY1 -> VARDECLORSTAT REPTFUNCBODY1");
    else
      success = false;
  } else if (in(follow_reptfuncbody1, 1)) {
    updateDerivation(&derivation_string, "REPTFUNCBODY1", "");
    print_rule("REPTFUNCBODY1 -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

/*
REPTIDNEST1 -> INDICE REPTIDNEST1
              |.
 */

int REPTIDNEST1() {

  if (!SkipErrors(first_reptidnest1, follow_reptidnest1, 2, 18, "REPTIDNEST1"))
    return false;
  if (in(first_indice, 1)) {

    updateDerivation(&derivation_string, "REPTIDNEST1", "INDICE REPTIDNEST1");
    if (INDICE() & REPTIDNEST1()) {

      print_rule("REPTIDNEST1 -> INDICE REPTIDNEST1");
    } else
      success = false;
  } else if (in(follow_reptidnest1, 18)) {

    updateDerivation(&derivation_string, "REPTIDNEST1", "");
    print_rule("REPTIDNEST1 -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

/*
REPTIMPLDEF3 -> FUNCDEF REPTIMPLDEF3 //
               |.
 */

int REPTIMPLDEF3() {

  if (!SkipErrors(first_reptimpldef3, follow_reptimpldef3, 2, 1,
                  "REPTIMPLDEF3"))
    return false;
  if (in(first_funcdef, 1)) {
    updateDerivation(&derivation_string, "REPTIMPLDEF3",
                     "FUNCDEF REPTIMPLDEF3");
    if (FUNCDEF() & REPTIMPLDEF3()) {
      print_rule("REPTIMPLDEF3 -> FUNCDEF REPTIMPLDEF3");

    } else {
      success = false;
    }
  } else if (in(follow_reptimpldef3, 1)) {
    updateDerivation(&derivation_string, "REPTIMPLDEF3", "");
    print_rule("REPTIMPLDEF3 -> EPSILON");
  } else {
    success = false;
  }
  return success;
}

/*
REPTOPTSTRUCTDECL22 -> comma id REPTOPTSTRUCTDECL22 //
                      |.
 */

int REPTOPTSTRUCTDECL22() {

  if (!SkipErrors(first_reptoptstructdecl22, follow_reptoptstructdecl22, 2, 1,
                  "REPTOPTSTRUCTDECL22"))
    return false;
  if (compare("comma")) {
    updateDerivation(&derivation_string, "REPTOPTSTRUCTDECL22",
                     "comma id REPTOPTSTRUCTDECL22");
    if (match("comma") & match("id") & REPTOPTSTRUCTDECL22())
      print_rule("REPTOPTSTRUCTDECL22 -> comma id REPTOPTSTRUCTDECL22");
    else
      success = false;
  } else if (in(follow_reptoptstructdecl22, 1)) {

    updateDerivation(&derivation_string, "REPTOPTSTRUCTDECL22", "");
    print_rule("REPTOPTSTRUCTDECL22 -> EPSILON");
  } else
    success = false;

  return success;
}

/*
REPTSTATBLOCK1 -> STATEMENT REPTSTATBLOCK1  //d
                 |.
 */
int STATEMENT();
int REPTSTATBLOCK1() {

  if (!SkipErrors(first_reptstatblock1, follow_reptstatblock1, 7, 1,
                  "REPTSTATBLOCK1"))
    return false;
  if (in(first_statement, 7)) {
    updateDerivation(&derivation_string, "REPTSTATBLOCK1",
                     "STATEMENT REPTSTATBLOCK1");
    if (STATEMENT() & REPTSTATBLOCK1())
      print_rule("REPTSTATBLOCK1 -> STATEMENT REPTSTATBLOCK1");
    else
      success = false;
  } else if (in(follow_reptstatblock1, 1)) {
    updateDerivation(&derivation_string, "REPTSTATBLOCK1", "");
    print_rule("REPTSTATBLOCK1 -> EPSILON");
  } else
    success = false;

  return success;
}

/*
REPTSTRUCTDECL4 -> VISIBILITY MEMBERDECL REPTSTRUCTDECL4 //
                  |.
 */
int VISIBILITY();
int REPTSTRUCTDECL4() {

  if (!SkipErrors(first_reptstructdecl4, follow_reptstructdecl4, 3, 1,
                  "REPTOPTSTRUCTDECL22"))
    return false;
  if (in(first_visibility, 2)) {
    updateDerivation(&derivation_string, "REPTSTRUCTDECL4",
                     "VISIBILITY MEMBERDECL REPTSTRUCTDECL4");
    if (VISIBILITY() & MEMBERDECL() & REPTSTRUCTDECL4()) {
      print_rule("REPTSTRUCTDECL4 -> VISIBILITY MEMBERDECL REPTSTRUCTDECL4");
    } else {
      success = false;
    }
  } else if (in(follow_reptstructdecl4, 1)) {
    updateDerivation(&derivation_string, "REPTSTRUCTDECL4", "");
    print_rule("REPTSTRUCTDECL4 -> EPSILON");
  } else {
    success = false;
  }
  return success;
}
/*
REPTVARDECL4 -> ARRAYSIZE REPTVARDECL4
               |.
 */

int REPTVARDECL4() {

  if (!SkipErrors(first_reptvardecl4, follow_reptvardecl4, 2, 1,
                  "REPTVARDECL4"))
    return false;

  if (in(first_arraysize, 1)) {
    updateDerivation(&derivation_string, "REPTVARDECL4",
                     "ARRAYSIZE REPTVARDECL4");
    if (ARRAYSIZE() & REPTVARDECL4())
      print_rule("REPTVARDECL4 -> ARRAYSIZE REPTVARDECL4");
    else
      success = false;
  } else if (in(follow_reptvardecl4, 1)) {

    updateDerivation(&derivation_string, "REPTVARDECL4", "");
    print_rule("REPTVARDECL4 -> EPSILON");

  } else {
    success = false;
  }

  return success;
}
/*
RETURNTYPE -> TYPE
             |void .
 */

int RETURNTYPE() {

  if (!SkipErrors(first_returntype, follow_returntype, 4, 2, "RETURNTYPE"))
    return false;
  if (in(first_type, 3)) {

    updateDerivation(&derivation_string, "RETURNTYPE", "TYPE");
    if (TYPE()) {
      print_rule("RETURNTYPE -> TYPE");
    } else {
      success = false;
    }
  } else if (compare("void")) {
    updateDerivation(&derivation_string, "RETURNTYPE", "void");
    if (match("void")) {
      node *temp = pop_node(stack);
      temp->type = type;
      push_node(temp, stack);
      print_rule("RETURNTYPE -> void");
    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}
/*
RIGHTRECARITHEXPR ->
                    |ADDOP TERM RIGHTRECARITHEXPR .
 */
int RIGHTRECARITHEXPR() {

  if (!SkipErrors(first_rightrecarithexpr, follow_rightrecarithexpr, 4, 10,
                  "RIGHTRECARITHEXPR"))
    return false;
  if (in(first_addop, 3)) {

    updateDerivation(&derivation_string, "RIGHTRECARITHEXPR",
                     "ADDOP TERM RIGHTRECARITHEXPR");

    if (ADDOP() & TERM()) {

      node *factor1 = pop_node(stack);
      print_stack(stack, stack_contents);
      node *multop = pop_node(stack);
      print_stack(stack, stack_contents);
      node *factor2 = pop_node(stack);
      print_stack(stack, stack_contents);

      push_node(factor2, stack);
      push_node(factor1, stack);

      node *temp = create_subtree_n_nodes(addop, 2, stack, line);
      temp->value = multop->value;

      push_node(temp, stack);
      print_stack(stack, stack_contents);
    } else {
      success = false;
    }
    if (RIGHTRECARITHEXPR()) {
      print_rule("RIGHTRECARITHEXPR -> ADDOP TERM RIGHTRECARITHEXPR");
    } else
      success = false;
  } else if (in(follow_rightrecarithexpr, 10)) {
    updateDerivation(&derivation_string, "RIGHTRECARITHEXPR", "");
    print_rule("RIGHTRECARITHEPR -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

/*
RIGHTRECTERM ->
               | MULTOP FACTOR RIGHTRECTERM .
 */

int RIGHTRECTERM() {

  if (!SkipErrors(first_rightrecterm, follow_rightrecterm, 4, 13,
                  "RIGHTRECTERM"))
    return false;
  if (in(first_multop, 3)) {

    updateDerivation(&derivation_string, "RIGHTRECTERM",
                     "MULTOP FACTOR RIGHTRECTERM");

    if (MULTOP() & FACTOR()) {

      node *factor1 = pop_node(stack);
      print_stack(stack, stack_contents);
      node *operation = pop_node(stack);
      print_stack(stack, stack_contents);
      node *factor2 = pop_node(stack);
      print_stack(stack, stack_contents);

      push_node(factor2, stack);
      push_node(factor1, stack);

      node *temp = create_subtree_n_nodes(multop, 2, stack, line);
      temp->value = operation->value;

      print_stack(stack, stack_contents);
      push_node(temp, stack);
      print_stack(stack, stack_contents);
    } else {
      success = false;
    }
    if (RIGHTRECTERM()) {
      print_rule("RIGHTRECTERM -> MULTOP FACTOR RIGHTRECTERM");
    } else
      success = false;
  } else if (in(follow_rightrecterm, 13)) {
    updateDerivation(&derivation_string, "RIGHTRECTERM", "");
    print_rule("RIGHTRECTERM -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

/*
SIGN -> plus
      |minus .
 */

int SIGN() {

  if (!SkipErrors(first_sign, follow_sign, 2, 7, "SIGN"))
    return false;
  if (compare("plus")) {
    updateDerivation(&derivation_string, "SIGN", "+");
    if (match("plus"))
      print_rule("SIGN -> plus");
    else
      success = false;
  } else if (compare("minus")) {
    updateDerivation(&derivation_string, "SIGN", "minus");
    if (match("minus"))
      print_rule("SIGN -> minus");
    else
      success = false;
  } else
    success = false;
  return success;
}

/*
STATBLOCK -> lcubr REPTSTATBLOCK1 rcubr
            |STATEMENT
            |.
 */

int STATBLOCK() {

  if (!SkipErrors(first_statblock, follow_statblock, 8, 2, "STATBLOCK"))
    return false;

  if (compare("lcubr")) {

    updateDerivation(&derivation_string, "STATBLOCK",
                     "lcubr REPTSTATBLOCK1 rcubr");

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    if (match("lcubr") & REPTSTATBLOCK1() & match("rcubr")) {

      push_node(create_subtree_pop_until(statblock, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      print_rule("STATBLOCK -> lcubr REPTSTATBLOCK1 rcubr");
    } else
      success = false;
  } else if (in(first_statement, 6)) {

    updateDerivation(&derivation_string, "STATBLOCK", "STATEMENT");
    if (STATEMENT()) {

      push_node(create_subtree_n_nodes(statblock, 1, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("STATBLOCK -> lcubr REPTSTATBLOCK1 rcubr");
    } else
      success = false;
  } else if (in(follow_statblock, 2)) {

    push_node(init_node(statblock), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "STATBLOCK", "");
    print_rule("STATBLOCK -> EPSILON");

  } else {
    success = false;
  }

  return success;
}

/*
STATEMENT -> id STATEMENTBODY
            |if lpar RELEXPR rpar then STATBLOCK else STATBLOCK semi
            |while lpar RELEXPR rpar STATBLOCK semi
            |read lpar VARIABLE rpar semi
            |write lpar EXPR rpar semi
            |return lpar EXPR rpar semi .

            Note that we cannot have any varaible declarations inside of a while
or if block so there is no point of giving scopes!
 */
int STATEMENTBODY();
int VARIABLE();
int STATEMENT() {

  if (!SkipErrors(first_statement, follow_statement, 6, 10, "STATEMENT"))
    return false;
  if (compare("id")) {

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "STATEMENT", "id STATEMENTBODY");
    if (match("id") & STATEMENTBODY())
      print_rule("STATEMENT -> id STATEMENTBODY");
    else
      success = false;
  } else if (compare("if")) {
    updateDerivation(&derivation_string, "STATEMENT",
                     "if lpar RELEXPR rpar then STATBLOCK else STATBLOCK");
    if (match("if") & match("lpar") & RELEXPR() & match("rpar") &
        match("then") & STATBLOCK() & match("else") & STATBLOCK() &
        match("semi")) {

      push_node(create_subtree_n_nodes(ifnode, 3, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("STATEMENT -> if lpar RELEXPR rpar then STATBLOCK else "
                 "STATBLOCK semi");
    } else
      success = false;
  } else if (compare("read")) {
    updateDerivation(&derivation_string, "STATEMENT",
                     "read lpar VARIABLE rpar semi");
    if (match("read") & match("lpar") & VARIABLE() & match("rpar") &
        match("semi")) {

      push_node(create_subtree_n_nodes(read, 1, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("STATEMENT -> read lpar VARAIBLE rpar semi");
    } else
      success = false;
  } else if (compare("while")) {
    updateDerivation(&derivation_string, "STATEMENT",
                     "while lpar RELEXPR rpar STATBLOCK semi");
    if (match("while") & match("lpar") & RELEXPR() & match("rpar") &
        STATBLOCK() & match("semi")) {

      push_node(create_subtree_n_nodes(whilenode, 2, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("STATEMENT -> while lpar RELEXPR rpar STATBLOCK semi");
    } else {
      success = false;
    }
  } else if (compare("write")) {
    updateDerivation(&derivation_string, "STATEMENT",
                     "write lpar EXPR rpar semi");
    if (match("write") & match("lpar") & EXPR() & match("rpar") &
        match("semi")) {

      push_node(create_subtree_n_nodes(write, 1, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("STATEMENT -> write lpar EXPR rpar semi");
    } else
      success = false;
  } else if (compare("return")) {
    updateDerivation(&derivation_string, "STATEMENT",
                     "return lpar EXPR rpar semi");
    if (match("return") & match("lpar") & EXPR() & match("rpar") &
        match("semi")) {

      push_node(create_subtree_n_nodes(returnnode, 1, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("STATEMENT -> return lpar EXPR rpar semi");
    } else
      success = false;
  } else {
    success = false;
  }
  return success;
}

/*
STATEMENTBODY -> REPTIDNEST1 CHECKFORENDVAR //next
                | lpar APARAMS rpar CHECKFORENDFUNC .
 */

int CHECKFORENDFUNC();
int CHECKFORENDVAR();

int STATEMENTBODY() {

  if (!SkipErrors(first_statementbody, follow_statementbody, 4, 10,
                  "STATEMENTBODY"))
    return false;
  if (in(first_reptidnest1, 2) || in(first_checkforendvar, 2)) {

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "STATEMENTBODY",
                     "REPTIDNEST1 CHECKFORENDVAR");

    if (REPTIDNEST1()) {

      push_node(create_subtree_pop_until(dimlist, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      push_node(create_subtree_n_nodes(var, 2, stack, line), stack);
      print_stack(stack, stack_contents);
    } else {
      success = false;
    }
    if (CHECKFORENDVAR()) {
      print_rule("STATEMENTBODY -> REPTIDNEST1 CHECKFORENDVAR");
    } else
      success = false;
  } else if (compare("lpar")) {
    updateDerivation(&derivation_string, "STATEMENTBODY",
                     "lpar APARAMS rpar CHECKFORENDFUNC");
    if (match("lpar") & APARAMS() & match("rpar")) {

      push_node(create_subtree_n_nodes(funccall, 2, stack, line), stack);
      print_stack(stack, stack_contents);
    }
    if (CHECKFORENDFUNC()) {

      print_rule("STATEMENTBODY -> lpar APARAMS rpar CHECKFORENDFUNC");
    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
VARFUNCSTATENEST -> dot id VARFUNCSTATENEST2 .

We have an issue here that we would like to create the dot at the end by calling
push(create_n(dot,2)), however because we may because we may be creating an
assignment we cannot just put this here.


 */

int VARFUNCSTATENEST2();
int VARFUNCSTATENEST() {

  if (!SkipErrors(first_varfuncstatenest, follow_varfuncstatenest, 1, 10,
                  "VARFUNCSTATENEST"))
    return false;
  if (compare("dot")) {
    updateDerivation(&derivation_string, "VARFUNCSTATENEST",
                     "dot id VARFUNCSTATENEST2");

    push_node(create_subtree_n_nodes(dot, 1, stack, line), stack);
    print_stack(stack, stack_contents);

    if (match("dot") & match("id") & VARFUNCSTATENEST2()) {
      print_rule("VARFUNCSTATENEST -> dot id VARFUNCSTATENEST2");
    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
VARFUNCSTATENEST2 -> lpar APARAMS rpar CHECKFORENDFUNC
                    | REPTIDNEST1 CHECKFORENDVAR .
 */

int VARFUNCSTATENEST2() {

  if (!SkipErrors(first_varfuncstatenest2, follow_varfuncstatenest2, 4, 10,
                  "VARFUNCSTATENEST2"))
    return false;
  if (compare("lpar")) {
    updateDerivation(&derivation_string, "VARFUNCSTATENEST2",
                     "lpar APARAMS rpar CHECKFORENDFUNC");
    if (match("lpar") & APARAMS() & match("rpar")) {

      push_node(create_subtree_n_nodes(funccall, 2, stack, line), stack);
      print_stack(stack, stack_contents);
    } else {
      success = false;
    }
    if (CHECKFORENDFUNC()) {
      print_rule("VARFUNCSTATENEST2 -> lpar APARAMS rpar CHECKFORENDFUNC");
    } else
      success = false;

  } else if (in(first_reptidnest1, 2) || in(first_checkforendvar, 2)) {
    updateDerivation(&derivation_string, "VARFUNCSTATENEST2",
                     "REPTIDNEST1 CHECKFORENDVAR");

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    if (REPTIDNEST1()) {

      push_node(create_subtree_pop_until(dimlist, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      push_node(create_subtree_n_nodes(var, 2, stack, line), stack);
      print_stack(stack, stack_contents);
    } else {
      success = false;
    }
    if (CHECKFORENDVAR()) {
      print_rule("VARFUNCSTATENEST2 -> REPTIDNEST1 CHECKFORENDVAR");
    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
CHECKFORENDFUNC -> semi  //
            | VARFUNCSTATENEST .
 */

int CHECKFORENDFUNC() {

  if (!SkipErrors(first_checkforendfunc, follow_checkforendfunc, 2, 10,
                  "CHECKFORENDFUNC")) {
    return false;
  }
  if (compare("semi")) {

    node *current = pop_node(stack);
    while (peek_stack(stack) == dot) {
      node *top = pop_node(stack);
      add_child(top, current);
      current = top;
    }

    if (peek_stack(stack) == sentinel) {
      node *temp = pop_node(stack);
      free(temp);
    }
    push_node(current, stack);

    updateDerivation(&derivation_string, "CHECKFORENDFUNC", "semi");
    if (match("semi"))
      print_rule("CHECKFORENDFUNC -> semi");
    else
      success = false;

  } else if (in(first_varfuncstatenest, 1)) {

    updateDerivation(&derivation_string, "CHECKFORENDFUNC", "VARFUNCSTATENEST");
    if (VARFUNCSTATENEST())
      print_rule("CHECKFORENDFUNC -> VARFUNCSTATENEST");
  } else {
    success = false;
  }
  return success;
}

/*
CHECKFORENDVAR -> ASSIGNOP EXPR semi
                | VARFUNCSTATENEST .
 */

int CHECKFORENDVAR() {
  if (!SkipErrors(first_checkforendvar, follow_checkforendvar, 2, 10,
                  "CHECKFORENDVAR"))
    return false;
  if (in(first_assignop, 1)) {
    updateDerivation(&derivation_string, "CHECKFORENDVAR",
                     "ASSIGNOP EXPR semi");

    node *current = pop_node(stack);
    while (peek_stack(stack) == dot) {
      node *top = pop_node(stack);
      add_child(top, current);
      current = top;
    }

    if (peek_stack(stack) == sentinel) {
      node *temp = pop_node(stack);
      free(temp);
    }
    push_node(current, stack);

    if (ASSIGNOP() & EXPR() & match("semi")) {

      push_node(create_subtree_n_nodes(assingop, 2, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("CHECKFORENDVAR -> ASSIGNOP EXPR semi");
    } else
      success = false;
  } else if (in(first_varfuncstatenest, 1)) {

    updateDerivation(&derivation_string, "CHECKFORENDVAR", "VARFUNCSTATENEST");
    if (VARFUNCSTATENEST()) {

      print_rule("CHECKFORENDVAR -> VARFUNCSTATENEST");
    } else
      success = false;
  } else {
    success = false;
  }
  return success;
}

/*
STRUCTDECL -> struct id OPTSTRUCTDECL2 lcubr REPTSTRUCTDECL4 rcubr semi .
 */

int STRUCTDECL() {

  if (!SkipErrors(first_structdecl, follow_structdecl, 1, 4, "STRUCTDECL"))
    return false;
  if (compare("struct")) {

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    updateDerivation(
        &derivation_string, "STRUCTDECL",
        "struct id OPTSTRUCTDECL2 lcubr REPTSTRUCTDECL4 rcubr semi");

    if (match("struct") & match("id") & OPTSTRUCTDECL2() & match("lcubr") &
        REPTSTRUCTDECL4() & match("rcubr") & match("semi")) {

      push_node(create_subtree_pop_until(structdecl, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      print_rule("STRUCTDECL -> id struct OPTSTRUCTDECL2 lcubr REPTSTRUCTDECL4 "
                 "recubr semi");
    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
STRUCTORIMPLORFUNC -> STRUCTDECL
                     |IMPLDEF
                     |FUNCDEF .
 */

int STRUCTORIMPLORFUNC() {

  if (!SkipErrors(first_structorimplorfunc, follow_structorimplorfunc, 3, 4,
                  "STRUCTORIMPLORFUNC"))

    return false;
  if (in(first_structdecl, 1)) {
    updateDerivation(&derivation_string, "STRUCTORIMPLORFUNC", "STRUCTDECL");
    if (STRUCTDECL()) {
      print_rule("STRUCTORIMPLORFUNC -> STRUCTDECL");
    } else {
      success = false;
    }
  } else if (in(first_impldef, 1)) {
    updateDerivation(&derivation_string, "STRUCTORIMPLORFUNC", "IMPLDEF");
    if (IMPLDEF()) {
      print_rule("STRUCTORIMPLORFUNC -> IMPLDEF");
    } else {
      success = false;
    }
  } else if (in(first_funcdef, 1)) {
    updateDerivation(&derivation_string, "STRUCTORIMPLORFUNC", "FUNCDEF");
    if (FUNCDEF()) {
      print_rule("STRUCTORIMPLORFUNC -> FUNCDEF");
    } else {
      success = false;
    }
  } else {
    success = false;
  }

  return success;
}

/*
TERM -> FACTOR RIGHTRECTERM .
 */

int TERM() {

  if (!SkipErrors(first_term, follow_term, 7, 13, "TERM"))
    return false;
  if (in(first_factor, 7)) {
    updateDerivation(&derivation_string, "TERM", "FACTOR RIGHTRECTERM");
    if (FACTOR() & RIGHTRECTERM())
      print_rule("TERM -> FACTOR RIGHTRECTERM");
    else
      success = false;
  } else {
    success = false;
  }
  return success;
}

/*
TYPE -> integer
       |float
       |id .

 */

int TYPE() {

  if (!SkipErrors(first_type, follow_type, 3, 5, "TYPE"))
    return false;
  if (compare("integer")) {
    updateDerivation(&derivation_string, "TYPE", "integer");
    if (match("integer")) {

      print_stack(stack, stack_contents);

      print_rule("TYPE -> integer");
    } else
      success = false;
  } else if (compare("float")) {

    updateDerivation(&derivation_string, "TYPE", "float");
    if (match("float")) {

      print_stack(stack, stack_contents);
      print_rule("TYPE -> float");
    } else
      success = false;
  } else if (compare("id")) {
    updateDerivation(&derivation_string, "TYPE", "id");
    if (match("id")) {

      node *temp = pop_node(stack);
      temp->type = type;
      push_node(temp, stack);
      print_stack(stack, stack_contents);

      print_rule("TYPE -> id");
    } else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
VARDECL -> let id colon TYPE REPTVARDECL4 semi .//
 */

int VARDECL() {

  if (!SkipErrors(first_vardecl, follow_vardecl, 1, 9, "VARDECL"))
    return false;

  if (compare("let")) {

    updateDerivation(&derivation_string, "VARDECL",
                     "let id colon TYPE REPTVARDECL4 semi");
    if (match("let") & match("id") & match("colon") & TYPE()) {

      push_node(init_node(sentinel), stack);
      print_stack(stack, stack_contents);

      if (REPTVARDECL4()) {

        push_node(create_subtree_pop_until(arraydims, sentinel, stack, line),
                  stack);
        print_stack(stack, stack_contents);
      } else {
        success = false;
      }
      if (match("semi"))
        print_rule("VARDECL -> let id TYPE colon TYPE REPTVARDECL4 semi");
      else
        success = false;
    } else
      success = false;
  } else
    success = false;

  return success;
}

/*
VARDECLORSTAT -> VARDECL
                |STATEMENT .
 */

int VARDECLORSTAT() {

  if (!SkipErrors(first_vardeclorstat, follow_vardeclorstat, 7, 8,
                  "VARDECLORSTAT"))
    return false;
  if (in(first_vardecl, 1)) {

    updateDerivation(&derivation_string, "VARDECLORSTAT", "VARDECL");
    if (VARDECL()) {

      push_node(create_subtree_n_nodes(vardecl, 3, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("VARDECLORSTAT -> VARDECL");
    } else {
      success = false;
    }
  } else if (in(first_statement, 6)) {
    updateDerivation(&derivation_string, "VARDECLORSTAT", "STATEMENT");
    if (STATEMENT())
      print_rule("VARDECLORSTAT -> STATEMENT");
    else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
VARIABLE -> id VARIABLE2 .//
 */

int VARIABLE2();

int VARIABLE() {

  if (!SkipErrors(first_variable, follow_variable, 1, 1, "VARIABLE"))
    return false;

  if (compare("id")) {
    updateDerivation(&derivation_string, "VARIABLE", "id VARIABLE2");
    if (match("id") & VARIABLE2())
      print_rule("VARIABLE -> id VARIABLE2");
    else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
VARIABLE2 ->  REPTIDNEST1 REPTVARIABLE    .
VARIABLE2 ->  lpar APARAMS rpar dot id VARIABLE2  .
 */

int REPTVARIABLE();
int VARIDNEST();

int VARIABLE2() {

  if (!SkipErrors(first_variable2, follow_variable2, 4, 1, "VARIABLE2"))
    return false;

  if (in(first_reptidnest1, 1) || in(first_reptvariable, 1)) {
    updateDerivation(&derivation_string, "VARIABLE2",
                     "REPTIDNEST1 REPTVARIABLE");

    push_node(init_node(sentinel), stack);
    print_stack(stack, stack_contents);

    if (REPTIDNEST1()) {

      push_node(create_subtree_pop_until(dimlist, sentinel, stack, line),
                stack);
      print_stack(stack, stack_contents);

      push_node(create_subtree_n_nodes(var, 2, stack, line), stack);
      print_stack(stack, stack_contents);

      if (REPTVARIABLE())
        print_rule("VARIABLE2 -> REPTIDNEST1 REPTVARIABLE");
      else
        success = false;

    } else
      success = false;
  } else if (compare("lpar")) {
    updateDerivation(&derivation_string, "VARIABLE2",
                     "lpar APARAMS rpar dot id VARIABLE2");
    if (match("lpar") & APARAMS() & match("rpar")) {

      push_node(create_subtree_n_nodes(funccall, 2, stack, line), stack);
      print_stack(stack, stack_contents);

      if (match("dot") & match("id") & VARIABLE2()) {
        push_node(create_subtree_n_nodes(dot, 2, stack, line), stack);
        print_stack(stack, stack_contents);
      }
      print_rule("VARIABLE2 -> lpar APARAMS rpar dot id VARIABLE2");
    } else
      success = false;
  } else if (in(follow_variable2, 1)) {

    push_node(init_node(dimlist), stack);

    push_node(create_subtree_n_nodes(var, 2, stack, line), stack);
    print_stack(stack, stack_contents);

    updateDerivation(&derivation_string, "VARIABLE2", "");
    print_rule("VARIABLE2 -> EPSILON");
  } else {
    success = false;
  }

  return success;
}

/*
REPTVARIABLE ->  dot id REPTVARIABLE   .
REPTVARIABLE ->  .
 */
int REPTVARIABLE() {
  if (!SkipErrors(first_reptvariable, follow_reptvariable, 2, 1,
                  "REPTVARIABLE"))
    return false;
  if (compare("dot")) {
    updateDerivation(&derivation_string, "REPTVARIABLE", "dot id VARIABLE2");
    if (match("dot") & match("id") & VARIABLE2()) {

      push_node(create_subtree_n_nodes(dot, 2, stack, line), stack);
      print_stack(stack, stack_contents);

      print_rule("REPTVARIABLE -> VARIDNEST REPTVARIABLE");
    } else
      success = false;
  } else if (in(follow_reptvariable, 1)) {

    updateDerivation(&derivation_string, "REPTVARIABLE", "");
    print_rule("REPTVARIABLE -> EPSILON");
  } else {
    success = false;
  }
  return success;
}

/*
VISIBILITY -> public
             |private .
 */

int VISIBILITY() {

  if (!SkipErrors(first_visibility, follow_visibility, 2, 2, "VISIBILITY"))
    return false;
  if (compare("public")) {
    updateDerivation(&derivation_string, "VISIBILITY", "public");
    if (match("public"))
      print_rule("VISIBILITY -> public");
    else
      success = false;
  } else if (compare("private")) {
    updateDerivation(&derivation_string, "VISIBILITY", "private");
    if (match("private"))
      print_rule("VISIBILITY -> private");
    else
      success = false;
  } else {
    success = false;
  }

  return success;
}

/*
 * Note that at the end of the parse the AST should be the only data structure
 * that has any dynamically allocated memory.
 */
