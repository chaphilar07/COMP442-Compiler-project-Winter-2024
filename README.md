This is the final version of the compiler, I was able to implement most of the features of the project.

What I am missing
  Floating point support, some object oriented features (for example shadowing member variables in the subclass is not supported)


To run the compiler I have created several toy test files to see if the compiler will work at creating the correct executable for the moon machine.

NOTE TO MAKE TESTING AND DEMONSTRATION EASIER I HAVE LEFT SEVERAL SOURCE FILES IN THE MAIN DIRECTORY, THIS JUST MAKES IT EASIER FOR DEMONSTRATION.


Run the command:

 ./COMPILER target.src

And this will produce an executable for the moon machine as well as several logging files and the AST output.

Note the output for the AST's etc are all in the output directory in their respective folders, the exception to this is logging information about the symbol table and the code generation that will all be produced in the main directory.


The actual source code for each of the parts is placed in their respective subdirectories inside of the src directory.


NOTE THE CODE IS A BIT OF A MESS! THE MAIN DIRECOTRY

- But if you follow the directions of the readme this code will compile and then we can run the resulting moon machine code.


