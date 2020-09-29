/*
    Name: Justin Igmen
    SID: 200364880
    Date: September 30, 2020
    Course: ENEL 487
    Assignment: 2

    Description: This program performs basic complex number arithmetic
*/

#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <fstream>

using namespace std;

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

    while (true) {
        real_ans = 0;
        imag_ans = 0;
        is_error = false;
        in_a_mode = false;
        output = "";

        printf("\nEnter exp: ");
        fgets(user_input, sizeof(user_input), stdin);

        if (strcmp(user_input, "q\n") == 0 || strcmp(user_input, "Q\n") == 0) {
            break;
        }

        check_interactive = sscanf_s(user_input, "%c %f %f %f %f", &sign, rsize_t{ 1 }, &z1.real, &z1.imag, &z2.real, &z2.imag);
        check_batch = sscanf_s(user_input, "%s < %s > %s", &path, rsize_t{ 100 }, 
            &in_filename, rsize_t{ 100 }, &out_filename, rsize_t{ 100 });

       
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
        
        if (check_batch == 3) {
            in_a_mode = true;

            strcpy_s(full_in_path, path);
            strcat_s(full_in_path, in_filename);

            strcpy_s(full_out_path, path);
            strcat_s(full_out_path, out_filename);

            ifstream fin(full_in_path);
            if (fin) {
                batch_mode(full_in_path, full_out_path);
            }
            else {
                fprintf(stderr, "Error: Incorrect format\n");
                fprintf(stderr, "Possible causes: Missing backslash at the end, invalid path, or file does not exist\n");
            }
        }

        if (!in_a_mode) {
            fprintf(stderr, "Invalid input.\n");
        }
    }

    printf("Program ended\n");
}

string solve(char sign, Complex z1, Complex z2, bool &error) {
    string output = "";
    float real_ans, imag_ans;
    bool div_by_zero = false;
    error = false;

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

    if (!error) {
        if (div_by_zero) {
            if (isnan(real_ans) && !isnan(imag_ans)) {
                if (imag_ans < 0) {
                    output = "NaN - j " + to_string(abs(imag_ans)) + "\n";
                }
                else {
                    output = "NaN + j " + to_string(imag_ans) + "\n";
                }
            }
            else if (!isnan(real_ans) && isnan(imag_ans)) {
                output = to_string(real_ans) + " + j NaN\n";
            }
            else if (isnan(real_ans) && isnan(imag_ans)) {
                output = "NaN + j NaN\n";
            }
        }
        else {
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

void batch_mode(char path[], char output_file[]) {
    int counter;
    Complex z1, z2;
    char user_input[200], sign;
    bool error = false;
    string output = "";
    FILE* in_file, * out_file;
    fopen_s(&in_file, path, "r");
    fopen_s(&out_file, output_file, "w");

    while (fgets(user_input, sizeof(user_input), in_file)) {
        counter = sscanf_s(user_input, "%c %f %f %f %f", &sign, rsize_t{ 1 }, &z1.real, &z1.imag, &z2.real, &z2.imag);
        output = solve(sign, z1, z2, error);

        if (user_input[0] == '\n') {
            fputs("\n", out_file);
            continue;
        }

        if (counter == 1 && (user_input[0] == 'q' || user_input[0] == 'Q')) {
            break;
        }

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


void add(Complex z1, Complex z2, float& real_ans, float& imag_ans) {
    real_ans = z1.real + z2.real;
    imag_ans = z1.imag + z2.imag;
}

void sub(Complex z1, Complex z2, float& real_ans, float& imag_ans) {
    real_ans = z1.real - z2.real;
    imag_ans = z1.imag - z2.imag;
}

void mul(Complex z1, Complex z2, float& real_ans, float& imag_ans) {
    real_ans = z1.real * z2.real;
    imag_ans = z1.imag * z2.imag;
}

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
