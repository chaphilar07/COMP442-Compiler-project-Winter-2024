/*
 * This is the driver file for the code generation part of the project.
 *
 * This source file will provide a driver that should produce an executable.
 */

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

      char src_path[1024];
      snprintf(src_path, sizeof(src_path), "tests/codegen/%s.src", name);

      // Parse the file, get AST root node.
      node *result = parse(src_path);

      // Check if parse was succesfull.
      if (result == NULL) {

        printf("FAILURE\n");

      } else {

        printf("SUCCESS\n");
      }
    }

    printf("END OF FILE \n\n\n\n");
  }

  printf("Done ... \n");
  return 0;
}
