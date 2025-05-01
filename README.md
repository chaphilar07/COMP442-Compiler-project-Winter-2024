This is the final version of the compiler, I was able to implement most of the features of the project.

What I am missing
  Floating point support, some object oriented features (for example shadowing member variables in the subclass is not supported)


To run the compiler I have created several toy test files to see if the compiler will work at creating the correct executable for the moon machine.


Run the command:

 ./COMPILER target.src

And this will produce an executable for the moon machine.

I have left several of these test files in the main directory, makes it easier for demonstration purposes.

The compiler produces the executable machine code, it also creates several files for error logging purposes (.log files)

The actual source code for each of the parts is placed in their respective subdirectories inside of the src directory.
