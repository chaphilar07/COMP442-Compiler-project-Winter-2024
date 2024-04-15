#include "src/CodeGeneration/CodeGeneration.h"
#include "src/SemanticAnalyzer//SemanticError.h"
#include "src/SemanticAnalyzer/SemanticError.h"
#include "src/SemanticAnalyzer/SymbolTable.h"
#include "src/lexer/Lexer.h"
#include "src/parser/AST/AST_SymbolTable.h"
#include "src/parser/parser.h"
#include "src/utils/utils.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "ERROR No input source file, exiting.\n");
    return -1;
  }

  const char *src_file_name = argv[1];

  const char *name = extractFileName(src_file_name);

  compile_regex();
  // Need to open the files for the parser.
  char output_path[124];         // Path to the output code file.
  char semantic_error_path[124]; // Path to the semantic errors file.
  char symbol_table_path[124];   // Path to symbol tables output.

  // Get the names for the files.
  snprintf(output_path, sizeof(output_path), "output/codegen/%s.m", name);
  snprintf(semantic_error_path, sizeof(semantic_error_path),
           "output/codegen/%s.outsemanticerrors", name);
  snprintf(symbol_table_path, sizeof(symbol_table_path),
           "output/codegen/%s.outsymboltable", name);

  // open the files.
  FILE *semantic_errors_file = fopen(semantic_error_path, "w+");
  FILE *output_file = fopen(output_path, "w+");
  FILE *symbol_table_file = fopen(symbol_table_path, "w+");

  // Parse the root node.
  node *root = parse(src_file_name);
  if (root) {
    ErrorArray *errors = init_errors();
    Scope *global_scope = create_program_scope(root, errors);
    print_errors(stderr, errors);
    print_scope(global_scope, symbol_table_file, 0);
    code_gen_pass(root, global_scope, output_file, errors);

    read_subtroutine(output_file);
    fprintf(output_file, "\n\n\n");
    write_subroutine(output_file);

    // Manage the state of the code generation module.
    free_all_registers();
    reset_index_pointers();

    fprintf(output_file, "parameterstorage res 512\n");
    fprintf(output_file, "indexstorage res 256 \n");

  } else {
    fprintf(stderr, "FAILURE COULD NOT PARSE FILE SYNTAX ERRORS!\n");
  }

  // Close the files
  fclose(semantic_errors_file);
  fclose(output_file);
  fclose(symbol_table_file);

  free_regexes();
  return 0;
}
