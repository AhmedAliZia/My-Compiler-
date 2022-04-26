# My Compiler
Project for course Compiler Construction. Comes with Lexical Analyzer, Parser, Three Address Code generator and Virtual Machine.
## How it works
The LexicalAnalyzer.cpp takes a code snippet as input. A sample code has been provided in "input.txt". It returns all the Token Lexeme Pairs in an output.txt file.
The Parser goes over the Token Lexeme pair checking for any semantic errors. It also generates the machine code for the Virtual machine. The Virtual Machine, implemented in 
VM.h, compiles the machine code and returns the output of the code. 
## Instructions for Running
1. Type the code in the input file. A sample code with syntax has been provided. 
2. Run LexicalAnalyzer.cpp, which will return a "output.txt" file with all the Token Lexeme pairs.
3. Parser.cpp will use that output.txt file. Run it next and it will compile the code in input.txt file. 
