Simple Compiler written in C/C++ for COMP442 2024 Winter semester.

The lexer reads each token there are 3 types of tokens ids, ints, floats and keywords.

Note that before using the lexer function to get the next token you MUST compile the regexs with the compile_regex() function, and when you no longer
need the regexes you can use free_regex() to free the memory.

The source files for Assignment 1 are in the src/ directory, the output for the tests that were provided are in the output/ directory.

To test the functionality of the lexer run the command in the src directory:

    make test



