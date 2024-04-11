/*
 * This is the driver file for the code generation part of the project.
 *
 * This source file will provide a driver that should produce an executable.
 */

#include "../SemanticAnalyzer/SemanticError.h"
#include "../parser/AST/AST_SymbolTable.h"
#include "../parser/parser.h"
#include "../utils/utils.h"
#include "CodeGeneration.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  DIR *dir;
  struct dirent *entry; // We want the tests in the code gen directory.

  dir = opendir("tests/codegen");

  if (dir == NULL) {
    perror("Cannot open tests directory, terminating. \n");
  }

  compile_regex(); // Always compile regex or tokenization breaks.

  while (((entry = readdir(dir)) != NULL)) {
    if (strstr(entry->d_name, ".src")) {
      char *file_name = entry->d_name;
      char *name = extractFileName(file_name);

      char src_path[124];            // Source file path
      char output_path[124];         // Path to the output code file.
      char semantic_error_path[124]; // Path to the semantic errors file.
      char symbol_table_path[124];

      snprintf(symbol_table_path, sizeof(symbol_table_path),
               "output/codegen/%s.outsymboltable", name);
      snprintf(src_path, sizeof(src_path), "tests/codegen/%s.src", name);
      snprintf(output_path, sizeof(output_path), "output/codegen/%s.m", name);
      snprintf(semantic_error_path, sizeof(semantic_error_path),
               "output/symboltable/%s.src", name);

      // Parse the file, get AST root node.
      node *result = parse(src_path);

      FILE *symbol_table_output = fopen(symbol_table_path, "w+");
      FILE *output_file = fopen(output_path, "w+");
      FILE *semantic_error_file = fopen(semantic_error_path, "w+");

      if (result) {

        ErrorArray *errors = init_errors();
        Scope *globalScope = create_program_scope(result, errors);
        print_errors(semantic_error_file, errors);
        print_scope(globalScope, symbol_table_output, 0);
        code_gen_pass(result, globalScope, output_file, errors);

        read_subtroutine(output_file);
        fprintf(output_file, "\n\n\n");
        write_subroutine(output_file);

        fprintf(output_file, "parameterstorage res 512\n");
        fprintf(output_file, "indexstorage res 256 \n");

      } else {
        fprintf(stderr, "FAILURE COULD NOT PARSE THE SOURCE FILE!\n");
        continue;
      }
    }
  }

  return 0;
}
