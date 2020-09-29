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


#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <cstring>

using namespace std;

//Data type for the complex number
class Complex {
public:
    float real;
    float imag;
};

string solve(char, Complex, Complex, bool&);
void batch_mode(char[], char[]);
void add(Complex, Complex, float&, float&);
void sub(Complex, Complex, float&, float&);
void mul(Complex, Complex, float&, float&);
void div(Complex, Complex, float&, float&);

int main() {
    Complex z1, z2;
    char user_input[200], sign;
    float real_ans, imag_ans;
    int check_interactive, check_batch;
    string output;
    char path[100], in_filename[100], out_filename[100], 
        full_in_path[260], full_out_path[260];
    bool is_error, in_a_mode;

    printf("Complex calculator\n\n");
    printf("Type a letter to specify the arithmetic operator (A, S, M, D)\n");
    printf("followed by two complex numbers expressed as pairs of doubles.\n");
    printf("Type Q to quit.\n");

    //Will run until user enters "Q" in the command
    while (true) {
        real_ans = 0;
        imag_ans = 0;
        is_error = false;
        in_a_mode = false;
        output = "";

        printf("\nEnter exp: ");
        fgets(user_input, sizeof(user_input), stdin);

        //Program ends when user enters 'q'
        if (strcmp(user_input, "q\n") == 0 || strcmp(user_input, "Q\n") == 0) {
            break;
        }

        //sscanf returns an int value based on the number of correct placeholders entered
        //The program will determine which mode it goes in based on the number returned
        check_interactive = sscanf(user_input, "%1c %f %f %f %f", &sign, &z1.real, &z1.imag, &z2.real, &z2.imag);
        check_batch = sscanf(user_input, "%s < %s > %s", &path, &in_filename, &out_filename);

        //Interactive mode
        if (check_interactive == 5) {
            in_a_mode = true;
            output = solve(user_input[0], z1, z2, is_error);
            if (is_error) {
                fprintf(stderr, "Invalid input.\n");
            }
            else {
                printf(output.c_str());
            }
        }
        
        //Batch mode
        if (check_batch == 3) {
            in_a_mode = true;

            //Concatenates the path and the input/output files
            strcpy(full_in_path, path);
            strcat(full_in_path, in_filename);
            strcpy(full_out_path, path);
            strcat(full_out_path, out_filename);

            FILE* in_file, * out_file;

            in_file = fopen(full_in_path, "r");
            out_file = fopen(full_out_path, "w");

            //Checks if the file exists
            if (in_file){
                batch_mode(full_in_path, full_out_path);
            }
            else {
                fprintf(stderr, "Error: Incorrect format\n");
                fprintf(stderr, "Possible causes: Missing backslash at the end of the path, invalid path, or input file does not exist\n");
            }
        }
        
        //If check_interactive or check_batch fails to get 5 or 3 respectively, the user entered incorrect input -- error
        if (!in_a_mode) {
            fprintf(stderr, "Invalid input.\n");
        }
    }

    printf("Program ended\n");
}

/*
Solves the command entered

Inputs:
sign: which operation to do
z1, z2: the complex numbers to solve
error: passed by reference, checks if there is an error in the operation

Output:
If an error is found (eg. sign is not a,s,m,d) it will return an empty string and set error to true
Otherwise, it will return a correct format of the answer in string
*/
string solve(char sign, Complex z1, Complex z2, bool &error) {
    string output = "";
    float real_ans, imag_ans;
    bool div_by_zero = false;
    error = false;

    //Checks which operation to use
    if (sign == 'a' || sign == 'A') {
        add(z1, z2, real_ans, imag_ans);
    }
    else if (sign == 's' || sign == 'S') {
        sub(z1, z2, real_ans, imag_ans);
    }
    else if (sign == 'm' || sign == 'M') {
        mul(z1, z2, real_ans, imag_ans);
    }
    else if (sign == 'd' || sign == 'D') {
        div(z1, z2, real_ans, imag_ans);
        if (z2.real == 0 || z2.imag == 0) {
            div_by_zero = true;
        }
    }
    else {
        output = "";
        error = true;
    }

    //Checks if there's error
    if (!error) {
        if (div_by_zero) {
            //Checks which components was divided by zero
            //Checks if NAN and formats the answer based on this
            if (isnan(real_ans) && !isnan(imag_ans)) {
                if (imag_ans < 0) {
                    output = "nan - j " + to_string(abs(imag_ans)) + "\n";
                }
                else {
                    output = "nan + j " + to_string(imag_ans) + "\n";
                }
            }
            else if (!isnan(real_ans) && isnan(imag_ans)) {
                output = to_string(real_ans) + " + j NaN\n";
            }
            else if (isnan(real_ans) && isnan(imag_ans)) {
                output = "nan + j nan\n";
            }
        }
        else {
            //Checks if imaginary component of the answer is negative and formats the answer accordingly
            if (imag_ans < 0) {
                output = to_string(real_ans) + " - j " + to_string(abs(imag_ans)) + "\n";
            }
            else {
                output = to_string(real_ans) + " + j " + to_string(imag_ans) + "\n";
            }
        }
    }
    return output;
}

/*
File processing part
Program will read the input file line by line, process the command, and write the result in the output file
Will loop until EOF or it reads "Q"

Inputs:
path: the input path
output_file: the output path
*/
void batch_mode(char path[], char output_file[]) {
    int counter;
    Complex z1, z2;
    char user_input[200], sign;
    bool error = false;
    string output = "";
    FILE* in_file, * out_file;

    in_file = fopen(path, "r");
    out_file = fopen(output_file, "w");

    //Reads the input file line by line
    while (fgets(user_input, sizeof(user_input), in_file)) {
        //Counter checks if the command is correct -- see main function for similar comment
        counter = sscanf(user_input, "%1c %f %f %f %f", &sign, &z1.real, &z1.imag, &z2.real, &z2.imag);
        output = solve(sign, z1, z2, error);

        //If there is whitespace, the program also adds whitespace to the output
        if (user_input[0] == '\n') {
            fputs("\n", out_file);
            continue;
        }

        //Breaks the loop if the program encounters 'q'
        if (counter == 1 && (user_input[0] == 'q' || user_input[0] == 'Q')) {
            break;
        }

        //Will write error message if command is invalid. Otherwise write answer
        if (error || counter < 5) {
            fputs("Invalid input.\n", out_file);
        }
        else {
            fputs(output.c_str(), out_file);
        }
    }

    printf("Reading inputs from %s\n", path);
    printf("Results written to %s\n", output_file);

    fclose(in_file);
    fclose(out_file);
}

//Addition
void add(Complex z1, Complex z2, float& real_ans, float& imag_ans) {
    real_ans = z1.real + z2.real;
    imag_ans = z1.imag + z2.imag;
}

//Subtraction
void sub(Complex z1, Complex z2, float& real_ans, float& imag_ans) {
    real_ans = z1.real - z2.real;
    imag_ans = z1.imag - z2.imag;
}

//Multiplication
void mul(Complex z1, Complex z2, float& real_ans, float& imag_ans) {
    real_ans = z1.real * z2.real;
    imag_ans = z1.imag * z2.imag;
}

//Division
//If division by zero, set answer to NAN
void div(Complex z1, Complex z2, float& real_ans, float& imag_ans) {

    if (z2.real == 0 && z2.imag != 0) {
        real_ans = NAN;
        imag_ans = z1.imag / z2.imag;
    }
    else if (z2.real != 0 && z2.imag == 0) {
        real_ans = z1.real / z2.real;
        imag_ans = NAN;
    }
    else if (z2.real == 0 && z2.imag == 0) {
        real_ans = NAN;
        imag_ans = NAN;
    }
    else {
        real_ans = z1.real / z2.real;
        imag_ans = z1.imag / z2.imag;
    }
}
