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

using namespace std;

class Complex {
public:
    float real;
    float imag;
};

void add(Complex, Complex, float&, float&);
void sub(Complex, Complex, float&, float&);
void mul(Complex, Complex, float&, float&);
void div(Complex, Complex, float&, float&);

int main() {
    Complex z1, z2;
    char user_input[200], sign, result[100];
    float real_ans, imag_ans;
    int test;

    bool div_by_zero, is_error, not_math, wrong_input;

    printf("Complex calculator\n\n");
    printf("Type a letter to specify the arithmetic operator (A, S, M, D)\n");
    printf("followed by two complex numbers expressed as pairs of doubles.\n");
    printf("Type Q to quit.\n");

    while (true) {
        real_ans = 0;
        imag_ans = 0;
        is_error = false;
        div_by_zero = false;
        not_math = false;
        wrong_input = false;

        printf("\nEnter exp: ");
        fgets(user_input, 100, stdin);

        if (strcmp(user_input, "q\n") == 0 || strcmp(user_input, "Q\n") == 0) {
            break;
        }

        if (sscanf_s(user_input, "%c %f %f %f %f", &sign, rsize_t{ 1 }, &z1.real, &z1.imag, &z2.real, &z2.imag) != 5) {
            wrong_input = true;
        }

        if (wrong_input) {
            printf("Invalid input.\n");
        }
        else {
            if (user_input[0] == 'a' || user_input[0] == 'A') {
                add(z1, z2, real_ans, imag_ans);
            }
            else if (user_input[0] == 's' || user_input[0] == 'S') {
                sub(z1, z2, real_ans, imag_ans);
            }
            else if (user_input[0] == 'm' || user_input[0] == 'M') {
                mul(z1, z2, real_ans, imag_ans);
            }
            else if (user_input[0] == 'd' || user_input[0] == 'D') {
                if (z1.imag == 0 || z2.imag == 0) {
                    is_error = true;
                    div_by_zero = true;
                }
                else {
                    div(z1, z2, real_ans, imag_ans);
                }
            }
            else {
                is_error = true;
                not_math = true;
            }
        }

        if (is_error || wrong_input) {
            if (not_math) {
                printf("Invalid input.\n");
            }
            if (div_by_zero) {
                printf("Illegal math operation involved (Division by zero).\n");
            }
        }
        else {
            if (imag_ans < 0) {
                printf("%f - j %f\n", real_ans, abs(imag_ans));
            }
            else {
                printf("%f + j %f\n", real_ans, imag_ans);
            }
        }
    }
}

void add(Complex z1, Complex z2, float& real_sum, float& imag_sum) {
    real_sum = z1.real + z2.real;
    imag_sum = z1.imag + z2.imag;
}

void sub(Complex z1, Complex z2, float& real_sum, float& imag_sum) {
    real_sum = z1.real - z2.real;
    imag_sum = z1.imag - z2.imag;
}

void mul(Complex z1, Complex z2, float& real_sum, float& imag_sum) {
    real_sum = z1.real * z2.real;
    imag_sum = z1.imag * z2.imag;
}

void div(Complex z1, Complex z2, float& real_sum, float& imag_sum) {
    real_sum = z1.real / z2.real;
    imag_sum = z1.imag / z2.imag;
}