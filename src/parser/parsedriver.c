#include "../utils/utils.h"
#include "parser.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  DIR *dir;
  struct dirent *entry;
  dir = opendir("tests/parser");
  if (dir == NULL) {
    perror("Cannot open tests directory, terminating. \n");
  }

  compile_regex();

  while (((entry = readdir(dir)) != NULL)) {
    if (strstr(entry->d_name, ".src")) {
      char *file_name = entry->d_name;
      char *name = extractFileName(file_name);

      char src_path[1024];
      snprintf(src_path, sizeof(src_path), "tests/parser/%s.src", name);

      printf("FILE %s \n", src_path);
      node *result = parse(src_path);

      if (result == NULL)
        printf("FAILURE\n");
      else
        printf("SUCCESS\n");
    }

    printf("END OF FILE \n\n\n\n");
  }

  printf("Done ... \n");
  return 0;
}
