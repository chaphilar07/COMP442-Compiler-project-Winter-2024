/*
 * This header file has the logic for generating the code and creating the table
 * entries necessary for the code generation part.
 *
 * We will define several functions to perform the semantic actions that will
 * generate the code.
 */

#include "CodeGeneration.h"
#include "../parser/AST/AST_SymbolTable.h"

/*
 * This function will set the size and offsets of all of the classes and class
 * functions in the global scope.
 *
 * Because we are hashing the entries when we insert there is no order, so we
 * must make sure that all of the class sizes are created before we begin
 * processing the functions for there size.
 *
 * Note that because there are no pointers in our language we do not have the
 * ability to have members of the same type as the containing class.
 */
