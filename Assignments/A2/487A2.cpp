/*
    Name: Justin Igmen
    SID: 200364880
    Date: September 30, 2020
    Course: ENEL 487
    Assignment: 2

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

//Data type for the complex number
class Complex {
public:
    float real;
    float imag;
};

Complex add(Complex, Complex);
Complex sub(Complex, Complex);
Complex mul(Complex, Complex);
Complex div(Complex, Complex);
Complex solve(char, Complex, Complex, bool&);
void print_result(Complex);
bool user_quit(char[]);


int main() {
    Complex z1, z2, answer;
    char user_input[LINE_BUFFER], sign;
    int check_interactive;
    bool is_error;
    bool is_done = false;

    fprintf(stderr, "Complex calculator\n\n");
    fprintf(stderr, "Type a letter to specify the arithmetic operator (A, S, M, D)\n");
    fprintf(stderr, "followed by two complex numbers expressed as pairs of doubles.\n");
    fprintf(stderr, "Type Q to quit.\n\n");

    //Will run until user enters "Q" in the command
    while (!is_done) {
        is_error = false;

        fprintf(stderr, "Enter exp: ");
        fgets(user_input, sizeof(user_input), stdin);
        check_interactive = sscanf(user_input, "%1c %f %f %f %f\n", &sign, &z1.real, &z1.imag, &z2.real, &z2.imag);

        if (user_quit(user_input)) {
            is_done = true;
        }
        else if (strcmp(user_input, "\n") == 0) {
            fprintf(stdout, "\n");
        }
        else {
            //Checks if input is entered properly
            if (check_interactive >= 5) {
                answer = solve(sign, z1, z2, is_error);
                if (is_error == true) {
                    fprintf(stderr, "Invalid operator.\n");
                }
                else {
                    print_result(answer);
                }
            }
            else {
                fprintf(stderr, "Invalid input.\n");
            }
        }
    }

    fprintf(stderr, "Program ended\n");
}

/*
Prints the answer and formats it
*/
void print_result(Complex answer) {
    //Checks if divided by zero
    if (isnan(answer.real) && isnan(answer.imag)) {
        fprintf(stdout, "nan + j nan\n");
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
Solves the command entered

Inputs:
sign: which operation to do
z1, z2: the complex numbers to solve
error: passed by reference, checks if there is an error in the operation

Output:
If an error is found (eg. sign is not a,s,m,d), bool error will be set
Returns the answer in complex format
*/
Complex solve(char sign, Complex z1, Complex z2, bool &error) {
    Complex answer;
    answer.real = 0;
    answer.imag = 0;
    error = false;

    //Checks which operation to use
    if (sign == 'a' || sign == 'A') {
        answer = add(z1, z2);
    }
    else if (sign == 's' || sign == 'S') {
        answer = sub(z1, z2);
    }
    else if (sign == 'm' || sign == 'M') {
        answer = mul(z1, z2);
    }
    else if (sign == 'd' || sign == 'D') {
        answer = div(z1, z2);
    }
    else {
        error = true;
    }
    return answer;
}

//Addition
Complex add(Complex z1, Complex z2) {
    Complex sum;
    sum.real = z1.real + z2.real;
    sum.imag = z1.imag + z2.imag;

    return sum;
}

//Subtraction
Complex sub(Complex z1, Complex z2) {
    Complex difference;
    difference.real = z1.real - z2.real;
    difference.imag = z1.imag - z2.imag;

    return difference;
}

//Multiplication
Complex mul(Complex z1, Complex z2) {
    Complex product;
    product.real = (z1.real * z2.real) - (z1.imag * z2.imag);
    product.imag = (z1.real * z2.imag) + (z2.real * z1.imag);

    return product;
}

//Division
//If division by zero, set answer to NAN
Complex div(Complex z1, Complex z2) {
    Complex quotient;
    if (z2.real == 0 && z2.imag == 0) {
        quotient.real = NAN;
        quotient.imag = NAN;
    }
    else {
        quotient.real = ((z1.real * z2.real) + (z1.imag * z2.imag)) / ((z2.real * z2.real) + (z2.imag * z2.imag));
        quotient.imag = ((z1.imag * z2.real) - (z1.real * z2.imag)) / ((z2.real * z2.real) + (z2.imag * z2.imag));
    }

    return quotient;
}