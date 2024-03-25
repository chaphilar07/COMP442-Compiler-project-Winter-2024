#include "../parser/AST/AST_SymbolTable.h"
#include "../parser/parser.h"
#include "../utils/utils.h"
#include "SemanticError.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

  // We need to open the files, get the AST, then create the tables, can copy
  // code from the last assignments.

  DIR *dir;
  struct dirent *entry;
  dir = opendir("tests/parser");
  if (dir == NULL) {
    perror("Cannot open tests directory, terminating. \n");
  }

  compile_regex(); // Need this for the lexer this validates the tokens.

  while (((entry = readdir(dir)) != NULL)) {
    if (strstr(entry->d_name, ".src")) {
      char *file_name = entry->d_name;
      char *name = extractFileName(file_name);

      char src_path[1024];
      char outputfile_symbol_table[1024];
      char tableEntryCreation[1024];
      char semanticErrors[1024];

      snprintf(semanticErrors, sizeof(semanticErrors),
               "output/symboltable/%s.semanticerrors", name);
      snprintf(tableEntryCreation, sizeof(tableEntryCreation),
               "output/symboltable/%s.symboltableentries", name);
      snprintf(src_path, sizeof(src_path), "tests/parser/%s.src", name);
      snprintf(outputfile_symbol_table, sizeof(outputfile_symbol_table),
               "output/symboltable/%s.outsymboltable", name);

      FILE *ST_ENTRIES = fopen(tableEntryCreation,
                               "w+"); // This will keep track of the order of
                                      // the insertions into the symbol tables.
      FILE *symbolTableOutput =
          fopen(outputfile_symbol_table,
                "w+"); // This file pointer will for the smybol tables output.

      FILE *semanticErrorsFile = fopen(semanticErrors, "w+");
      ErrorArray *arr = init_errors();

      // We get the AST from the parse function.
      node *result = parse(
          src_path); // The result returned from the parser is an AST root node.

      if (result != NULL) {
        Scope *globalScope = create_program_scope(result, ST_ENTRIES, arr);
        print_errors(semanticErrorsFile, arr);
        if (globalScope) {
          print_scope(globalScope, symbolTableOutput, 0);
          fprintf(ST_ENTRIES, "success\n");
        }

      } else {

        printf("Failure \n");
        fprintf(stderr,
                "Invalid program could not generate symbol tables ... \n");
      }
    }
  }

  return 0;
}
