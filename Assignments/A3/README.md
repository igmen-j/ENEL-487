# Assignment 3 README

This program performs basic complex number arithmetic.  
It adds more functionality from Assignment 2 found [here](https://github.com/igmen-j/ENEL-487/tree/master/Assignments/A2)  
  
It has two modes: interactive mode and batch mode.  
_Interactive mode_ allows the user to enter command directly from the terminal and the program will print the result.  
_Batch mode_ allows to user to redirect stdio to a file.
  
The program is case sensitive.  

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

Users may also enter the commands _abs_, _arg_, _argdeg_, _exp_, and _inv_  
_abs_ : Returns the magnitude (or modulus) of the given complex number, |z|.  
_arg_ : Returns the angle part (or argument) of the given complex number, in
radians.  
_argDeg_ : Returns the argument of the given complex number, in degrees.  
_exp_ : For the given complex number, z, calculate the exponential of that
number, e<sup>z<sup>.  
inv Return the reciprocal of the given complex number: 1 / z.  
  
An example of the command would then be:  
(1e3 + 2j) + (3 - 4.5j)
```bash
Enter exp: a 1e3 2 3 -4.5
1003.000000 - j 2.500000
```  
or  
```bash
Enter exp: arg 5.7 5.7
8.06102
```  

To run in _batch mode_, the user must enter command below:  
```bash
.\yourexecutable < inputfile.txt > outputfile.txt
```  

To quit, enter q  

## Error checks
This program handles errors in different ways:  
- Division be zero: 
```bash
Enter exp: d 1 2 0 0
Error: Division by zero.
```
- Incorrect input:
```bash
Enter exp: p 1 2 3 4
Error: Invalid input.
```

- Incorrect format: 
If the command does not have five or three correct inputs (based on the operator), the program will give an error message.  
```bash
Enter exp: a 1
Error: Required input for this operator is 5.
```
