/*
 * This file will define the subroutines for dealing with floating point
 * operations.
 */

#include "../SemanticAnalyzer/SemanticAnalyzer.h"
#include "../SemanticAnalyzer/SemanticError.h"
#include "../SemanticAnalyzer/SymbolTable.h"
#include "../parser/AST/AST_SymbolTable.h"
#include "CodeGeneration.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * We need functions to define the subroutines to handle the following:
 * 1. assignments.
 * 2. addition,subtraction, multiplication, division etc.
 * 3. lt,gt,leq,geq,eq,neq
 */

/*
 * What do these subroutines have to do exactly?
 */

/*
 * This function will assign a float into the correct location.
 */
void assign_float(node *astnode, Scope *globalScope, ErrorArray *errors,
                  FILE *out) {}
