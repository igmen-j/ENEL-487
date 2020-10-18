/*
    Name: Justin Igmen
    SID: 200364880
    Due Date: October 20, 2020
    Course: ENEL 487
    Assignment: 3

    Description: This program performs basic complex number arithmetic
*/

#define _CRT_SECURE_NO_DEPRECATE
/**
    I keep getting error message about need to use the *_s command on some lines
    eg) sscanf -> sscanf_s
    When I got my friend to test my code, he is getting error messages regarding those lines
    I looked up online and this preprocessor command fixes the issue
    With this, I can now use the original format of some of the commands
**/

#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

#define LINE_BUFFER 200
#define PI 3.14159265359
#define BASIC_MATH_INPUT 5
#define COMPLEX_MATH_INPUT 3

/**
    Data type for the complex number
    Two variables: real and imag which corresponds to the real and imaginary number of the complex number
**/
class Complex {
public:
    double real;
    double imag;
};

Complex add(Complex, Complex);
Complex sub(Complex, Complex);
Complex mul(Complex, Complex);
Complex div(Complex, Complex, bool&);
double abs(Complex);
double arg(Complex, bool&);
double argDeg(Complex, bool&);
Complex exp(Complex);
Complex inv(Complex, bool&);
Complex solve(char[], Complex, Complex, bool&, bool&, int);
void print_result(char[], Complex);
bool user_quit(char[]);
bool is_basic_math(char[]);
bool is_complex_math(char[]);


int main() {
    Complex z1, z2, answer;
    char user_input[LINE_BUFFER], command[LINE_BUFFER], input_overflow[LINE_BUFFER];
    int check_interactive;
    bool is_error;
    bool div_by_zero;
    bool is_done = false;

    fprintf(stderr, "Complex number calculator\n\n");

    //Will run until user enters "Q" in the command
    while (!is_done) {
        is_error = false;
        div_by_zero = false;

        fprintf(stderr, "Enter exp: ");
        fgets(user_input, sizeof(user_input), stdin);
        check_interactive = sscanf(user_input, "%s %lf %lf %lf %lf %s\n", command, &z1.real, &z1.imag, 
            &z2.real, &z2.imag, input_overflow);

        if (user_quit(user_input)) {
            is_done = true;
        }
        else if (strcmp(user_input, "\n") == 0) {
            fprintf(stdout, "\n");
        }
        else {
            //Checks if input is entered properly
            if (check_interactive == COMPLEX_MATH_INPUT || check_interactive == BASIC_MATH_INPUT) {
                answer = solve(command, z1, z2, is_error, div_by_zero, check_interactive);
                if (is_error == true) {
                    //will print an error message if user enters an incorrect operator
                    if (is_basic_math(command)) {
                        fprintf(stderr, "Error: Required input for this operator is 5.\n");
                    }
                    else if (is_complex_math(command)) {
                        fprintf(stderr, "Error: Required input for this operator is 3.\n");
                    }
                    else {
                        fprintf(stderr, "Error: Invalid operator.\n");
                    }
                }
                else {
                    if (div_by_zero == true) {
                        fprintf(stderr, "Error: Division by zero.\n");
                    }
                    else {
                        print_result(command, answer);
                    }
                }
            }
            else {
                if (check_interactive > BASIC_MATH_INPUT && is_basic_math(command)) {
                    fprintf(stderr, "Error: Required input for this operator is 5.\n");
                }
                else if (check_interactive > COMPLEX_MATH_INPUT && is_complex_math(command)) {
                    fprintf(stderr, "Error: Required input for this operator is 3.\n");
                }
                else {
                    fprintf(stderr, "Error: Invalid input.\n");
                }
            }
        }
    }
    fprintf(stderr, "Program ended\n");
}

/*
Prints the answer and formats it
Inputs:
    Command - this is the operastor
    Answer - the answer of the operation
If the operator is abs, arg, or argdeg, print only one number
Otherwise, print in rectagular form
*/
void print_result(char command[], Complex answer) {

    if (strcmp(command, "abs") == 0 || strcmp(command, "arg") == 0 || strcmp(command, "argdeg") == 0) {
        fprintf(stdout, "%.5e\n", answer.real);
    }
    else {
        //Checks if imaginary component of the answer is negative and formats the answer accordingly
        if (answer.imag < 0) {
            answer.imag *= -1;
            fprintf(stdout, "%.5e - j %.5e\n", answer.real, answer.imag);
        }
        else {
            fprintf(stdout, "%.5e + j %.5e\n", answer.real, answer.imag);
        }
    }
}

/*
Checks if the user has entered 'q' or 'Q' as the command
Input: user_input - the whole command entered by the user
*/
bool user_quit(char user_input[]) {
    if (strcmp(user_input, "q\n") == 0 || strcmp(user_input, "Q\n") == 0 ||
        strcmp(user_input, "q") == 0 || strcmp(user_input, "Q") == 0) {
        return true;
    }
    else {
        return false;
    }
}

/*
Checks if the operator is A, S, M, or D
Returns true if so and false otherwise
*/
bool is_basic_math(char command[]) {
    if (strcmp(command, "a") == 0 || strcmp(command, "A") == 0 || strcmp(command, "s") == 0 || 
        strcmp(command, "S") == 0 || strcmp(command, "m") == 0 || strcmp(command, "M") == 0 || 
        strcmp(command, "d") == 0 || strcmp(command, "D") == 0) {
        return true;
    }
    return false;
}

/*
Checks if the operator is abs, arg, argdeg, exp. inv
Returns true if so and false otherwise
*/
bool is_complex_math(char command[]) {
    if (strcmp(command, "abs") == 0 || strcmp(command, "arg") == 0 || strcmp(command, "argdeg") == 0 ||
        strcmp(command, "exp") == 0 || strcmp(command, "inv") == 0) {
        return true;
    }
    return false;
}

/*
Solves the command entered

Inputs:
sign: which operation to do
z1, z2: the complex numbers to solve
error: passed by reference, checks if there is an error in the operation

Output:
If an error is found, bool error will be set
If division by zero, bool div_by_zero will be set
Returns the answer in complex format
*/
Complex solve(char sign[], Complex z1, Complex z2, bool& error, bool& div_by_zero, int num_inputs) {
    Complex answer;
    answer.real = 0;
    answer.imag = 0;
    error = false;
    div_by_zero = false;
    
    //Checks which operation to use
    //BASIC_MATH_INPUT: user has entered 5 inputs
    if (num_inputs == BASIC_MATH_INPUT) {
        if (strcmp(sign, "a") == 0 || strcmp(sign, "A") == 0) {
            answer = add(z1, z2);
        }
        else if (strcmp(sign, "s") == 0 || strcmp(sign, "S") == 0) {
            answer = sub(z1, z2);
        }
        else if (strcmp(sign, "m") == 0 || strcmp(sign, "M") == 0) {
            answer = mul(z1, z2);
        }
        else if (strcmp(sign, "d") == 0 || strcmp(sign, "D") == 0) {
            answer = div(z1, z2, div_by_zero);
        }
        else {
            error = true;
        }
    }
    //COMPLEX_MATH_INPUT: user has entered 3 inputs
    else if (num_inputs == COMPLEX_MATH_INPUT) {
        if (strcmp(sign, "abs") == 0) {
            answer.real = abs(z1);
        }
        else if (strcmp(sign, "arg") == 0) {
            answer.real = arg(z1, div_by_zero);
        }
        else if (strcmp(sign, "argdeg") == 0) {
            answer.real = argDeg(z1, div_by_zero);
        }
        else if (strcmp(sign, "exp") == 0) {
            answer = exp(z1);
        }
        else if (strcmp(sign, "inv") == 0) {
            answer = inv(z1, div_by_zero);
        }
        else {
            error = true;
        }
    }
    else {
        error = true;
    }
    return answer;
}

/*
Adds the two complex numbers entered by the user
Returns the sum
*/
Complex add(Complex z1, Complex z2) {
    Complex sum;
    sum.real = z1.real + z2.real;
    sum.imag = z1.imag + z2.imag;
    return sum;
}

/*
Subtracts the two complex numbers entered by the user
Returns the difference
*/
Complex sub(Complex z1, Complex z2) {
    Complex difference;
    difference.real = z1.real - z2.real;
    difference.imag = z1.imag - z2.imag;
    return difference;
}

/*
Multiplies the two complex numbers entered by the user
Returns the product
*/
Complex mul(Complex z1, Complex z2) {
    Complex product;
    product.real = (z1.real * z2.real) - (z1.imag * z2.imag);
    product.imag = (z1.real * z2.imag) + (z2.real * z1.imag);
    return product;
}

/*
Divides the two complex numbers entered by the user
Returns the quotient

If the user has entered zero for the second complex number, this will set div_by_zero to true
    and will return zero for the complex number
*/
Complex div(Complex z1, Complex z2, bool &div_by_zero) {
    Complex quotient;
    if (z2.real == 0 && z2.imag == 0) {
        div_by_zero = true;
        return z2;
    }
    else {
        quotient.real = ((z1.real * z2.real) + (z1.imag * z2.imag)) / ((z2.real * z2.real) + (z2.imag * z2.imag));
        quotient.imag = ((z1.imag * z2.real) - (z1.real * z2.imag)) / ((z2.real * z2.real) + (z2.imag * z2.imag));
    }
    return quotient;
}

//Returns the magnitude of the input complex number
double abs(Complex z1) {
    return sqrt(pow(z1.real, 2) + (pow(z1.imag, 2)));
}

/*
Returns the angle of the input complex number in radians
If the real number is zero, it will set div_by_zero to true and will return zero
*/
double arg(Complex z1, bool& div_by_zero) {
    if (z1.real == 0) {
        div_by_zero = true;
        return z1.real;
    }
    return atan(z1.imag / z1.real);
}

/*
Returns the angle of the input complex number in degrees
If the real number is zero, it will set div_by_zero to true and will return zero

Converts the answer to positive degree
*/
double argDeg(Complex z1, bool& div_by_zero) {
    double answer;
    if (z1.real == 0) {
        div_by_zero = true;
        return z1.real;
    }
    answer = atan(z1.imag / z1.real) * 180 / PI;
    if (z1.real < 0 && z1.imag > 0) {
        answer += 180;
    }
    else if (z1.real < 0 && z1.imag < 0) {
        answer -= 180;
    }
    return answer;
}

//Returns the exponential of the input number = e^z
Complex exp(Complex z1) {
    Complex answer;
    answer.real = exp(z1.real) * cos(z1.imag);
    answer.imag = exp(z1.real) * sin(z1.imag);
    return answer;
}

/*
Return the reciprocal of the input complex number
If the user has entered zero for the complex number, this will set div_by_zero to true
    and will return zero for the complex number
*/
Complex inv(Complex z1, bool& div_by_zero) {
    Complex answer;
    if (z1.real == 0 && z1.imag == 0) {
        div_by_zero = true;
        return z1;
    }
    answer.real = z1.real / (pow(z1.real, 2) + pow(z1.imag, 2));
    answer.imag = -1 * (z1.imag / (pow(z1.real, 2) + pow(z1.imag, 2)));
    return answer;
}