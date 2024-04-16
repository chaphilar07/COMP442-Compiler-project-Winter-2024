/*
 * Ths file will contain functions that will be used for reporting semantic
 * errors.
 */

#include "SemanticError.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// This function prints a semantic error for us.

SemanticError create_error(const char *msg, err_code code, int line) {
  SemanticError err;
  err.code = code;
  err.line = line;

  char buffer[248]; // reserve buffer of 248 chars for the error message.
  if (code == err111) {
    snprintf(
        buffer, sizeof(buffer),
        "SEMANTIC ERROR LINE %d: Cannot lookup variable/function before it has "
        "been declared/defined, on line %d, %s used before declaration",
        line, line, msg);

    err.msg = strdup(buffer);
  } else if (code == err101) {
    snprintf(
        buffer, sizeof(buffer),
        "SEMANTIC ERROR ON LINE %d: Cannot implement functions for class that "
        "does not exist, class %s does not exist cannot implement .",
        line, msg);
    err.msg = strdup(buffer);
  } else if (code == err200) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Cannot inherit a class that does not "
             "exist %s does not exist.",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err202) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Cannot call function that has not "
             "been defined, function %s has not been defined ",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err203) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Cannot implement a member function "
             "that has not been declared, function %s has not been declared.",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err204) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR on LINE %d: Cannot call function %s with "
             "different number of parameters exiting,",
             line, msg);
  } else if (code == err205) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Cannot declare variable with type "
             "that does not exist, declaration of variable %s, with type that "
             "does not exist",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err206) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Cannot have function parameters "
             "declared with type that does not exist, fparam %s onl line %d, "
             "declared with non-existent type.",
             line, msg, line);
    err.msg = strdup(buffer);
  } else if (code == err207) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Cannot have function that returns "
             "non-existent type, function %s on line %d, returns type that "
             "does not exist",
             line, msg, line);
    err.msg = strdup(buffer);
  } else if (code == err800) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Duplicate use of identifier %s in "
             "the same scope exiting",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err900) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERRROR ON LINE %d: Member function %s that has "
             "been declared but has not been defined.",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err701) {
    snprintf(
        buffer, sizeof(buffer),
        "SEMANTIC ERROR ON LINE %d: Cannot access a member from a non-class "
        "type or non-existent class type, %s is not a class type",
        line, msg);
    err.msg = strdup(buffer);
  } else if (code == err501) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Error on operator %s, cannot use %s "
             "on two different types",
             line, msg, msg);
    err.msg = strdup(buffer);

  } else if (code == err702) {
    snprintf(
        buffer, sizeof(buffer),
        "SEMANTIC ERROR ON LINE %d: No member called %s belongs to the class "
        "being accessed",
        line, msg);
    err.msg = strdup(buffer);
  } else if (code == err901) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Assignment cannot occur between a "
             "variable and an expression of another type, %s is not the "
             "expected type for the assignment.",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err903) {
    snprintf(
        buffer, sizeof(buffer),
        "SEMANTIC ERROR ON LINE %d: Variable accessed with wrong number of "
        "dimensions, variable %s does not have that many array dimensions",
        line, msg);
    err.msg = strdup(buffer);
  } else if (code == err1101) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Cannot have a return statement in "
             "function with return type of void",
             line);
    err.msg = strdup(buffer);
  } else if (code == err1102) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Invalid return type, %s is the "
             "expected return type from the function.",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err1401) {

    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Invalid function call, function %s is "
             "not defined in the source file.",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err1402) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Function %s called with the incorrect "
             "number of arguments .",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err1403) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Function %s called  with incorrect "
             "type of arugments.",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err2100) {
    snprintf(
        buffer, sizeof(buffer),
        "SEMANTIC ERROR ON LINE %d: Trying to access array variable %s with "
        "variable/function call that is not of type integer.",
        line, msg);
    err.msg = strdup(buffer);
  } else if (code == err0001) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR: Error member %s is declared of a class type that "
             "is of a subclass, this is not possible.",
             msg);
    err.msg = strdup(buffer);
  } else if (code == war100) {
    snprintf(
        buffer, sizeof(buffer),
        "WARNING! LINE %d MEMBER VARIABLE %s IS SHADOWING AN INHERITED MEMBER!",
        line, msg);
    err.msg = strdup(buffer);

  } else if (code == err1200) {
    snprintf(buffer, sizeof(buffer),
             "WARNING! ON LINE %d: FUNCTION %s IS OVERRIDING INHERITED "
             "FUNCTION WITH DIFFERENT NUMBER OF PARAMETERS!",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err1201) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Cannot override a function with "
             "different types of parameters, function %s is being overriden "
             "with the wrong number of parameters.",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == war101) {
    snprintf(buffer, sizeof(buffer),
             "WARNING! FUNCTION %s IS BEING OVERRIDEN ON LINE %d !", msg, line);
    err.msg = strdup(buffer);
  } else if (code == war102) {
    snprintf(
        buffer, sizeof(buffer),
        "WARNING! LINE %d, LOCAL VARIABLE %s IN MEMBER FUNCTION IS SHADOWING "
        "A CLASS MEMBER!",
        line, msg);
    err.msg = strdup(buffer);
  } else if (code == err1410) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: FUNCTION %s BEING DEFIND WITH "
             "DIFFERENT NUMBER OF PARAMETERS ERROR !",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err1411) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: FUNCTION DEFINITION %s HAS DIFFERENT "
             "TYPES OF VARIABLES",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err1412) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: FUNCTION %s HAS BEEN DEFINED WITH A "
             "DIFFERENT RETURN TYPE",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err3100) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: NON-VOID FUNCTION MUST RETURN A "
             "VALUE, FUNCTION %s HAS NO RETURN STATEMENT.",
             line, msg);
    err.msg = strdup(buffer);
  } else if (code == err5100) {
    snprintf(buffer, sizeof(buffer),
             "SEMANTIC ERROR ON LINE %d: Last statement in function %s that is "
             "supposed to return is not a return statement exiting.",
             line, msg);
    err.msg = strdup(buffer);
  }
  return err;
}
// Comparator function for qsort, this is to sort the errors based on their
// line numbers.
int compare_error_by_line(const void *err1, const void *err2) {

  const SemanticError *error1 = (const SemanticError *)err1;
  const SemanticError *error2 = (const SemanticError *)err2;

  return (error1->line - error2->line);
}

int insert_error(ErrorArray *arr, SemanticError err) {
  if (!arr) {
    fprintf(stderr, "ERROR - insert_error(): Could not allocate memory for the "
                    "erorr array exiting\n");
    return -1;
  }

  arr->errors =
      realloc(arr->errors, sizeof(SemanticError) * (arr->currentSize + 1));
  if (!arr) {
    fprintf(stderr, "ERROR - insert_error(): Could not allocate memory for the "
                    "new error entry, exiting\n");
    return -1;
  }

  arr->errors[arr->currentSize] = err;
  arr->currentSize += 1;

  return 1;
}

void sort_errors_by_line(ErrorArray *arr) {
  if (!arr) {
    fprintf(
        stderr,
        "ERROR - sort_errors_by_line(): Cannot sort, array not allocated.\n");
    return;
  }
  if (arr->errors == NULL) {
    // This just means that we have no errors in our array.
    return;
  }

  qsort(arr->errors, arr->currentSize, sizeof(SemanticError),
        compare_error_by_line);
}

ErrorArray *init_errors() {
  ErrorArray *arr = malloc(sizeof(ErrorArray));
  if (!arr) {
    fprintf(stderr, "ERROR - init_errors(): Cannot allocate memory for an "
                    "array of errors, exiting\n");
    return NULL;
  }

  arr->currentSize = 0;
  arr->errors = NULL;

  return arr;
}

// This will print an array of semantic errors, to an output stream.
void print_errors(FILE *out, ErrorArray *err) {
  if (!out) {
    fprintf(stderr, "ERROR - print_errors(): Cannot print errors output "
                    "stream is null.\n");
    return;
  }
  if (!err) {
    fprintf(stderr, "ERROR - print_errors(): Cannot print errors output "
                    "stream is null.\n");
    return;
  }

  sort_errors_by_line(err);
  for (int i = 0; i < err->currentSize; i++) {
    fprintf(out, "%s\n", err->errors[i].msg);
    free((void *)err->errors[i].msg);
  }

  fprintf(out, "End of Error Report, total of %d errors/warnings found.\n",
          err->currentSize);

  free(err);
}
