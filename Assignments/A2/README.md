# Assignment 2 README

This program performs basic complex number arithmetic.  
It has two modes: interactive mode and batch mode.  
_Interactive mode_ allows the user to enter command directly from the terminal and the program will print the result.  
_Batch mode_ allows to user to redirect stdio to a file.
  
The program is not case sensitive.  

## Installation

To install:  
```bash
g++ -o yourfilenamehere.exe 487A2.cpp
```

## Run

To run in _interactive mode_, the user must enter five inputs.
The first component is the operation in the form of A, S, M, or D (addition, subtraction, multiplication ,or
division). This is not case-sensitive.  
The next two are the first complex number.  
The last two are the second complex number.  
  
The an example of the command would then be:  
(1e3 + 2j) + (3 - 4.5j)
```bash
Enter exp: a 1e3 2 3 -4.5
1003.000000 - j 2.500000
```  

To run in _batch mode_, the user must enter command below:  
```bash
.\yourexecutable < inputfile.txt > outputfile.txt
```  

To quit

## Error checks
This program handles errors in different ways:  
- Division be zero: When the user enters a command with division by zero, the program will output the result as nan.  
```bash
Enter exp: d 1 2 0 0
nan + j nan
```
- Incorrect operator:
```bash
Enter exp: p 1 2 3 4
Invalid operator.
```

- Incorrect format: 
If the command does not have five correct inputs, the program will give an error message.  
```bash
Enter exp: a 1
Invalid input.
```
Limitation: If the first five (interactive mode) or first three (batch mode) are correct, the program will read it as correct despite it having more int the command  
```bash
Enter exp: a 1 2 3 4 the program will ignore anything after the fifth input
4.000000 + j 6.000000
```
