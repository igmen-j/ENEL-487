/**********************************************************************
 *
 * Filename:    main.c
 * 
 * Description: A simple test program for the CRC implementations.
 *
 * Notes:       To test a different CRC standard, modify crc.h.
 *
 * 
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

/**********************************************************************
 Author: Justin Igmen
 SID: 200364880
 Class: ENEL 487
 Assignment: 4

 Purpose: The purpose of this assignment is to evaluate the speed of two CRC 
	algorithms. It uses the chrono library since it has a higher resolution 
	compared to the clock library.

 Changes: crc.c is converted to a .cpp file and I have added ASSERT_VALUE definition
	for each algorithm in the crc.h file. This is used for the automated unite test
	code section of the assignment.
	
	All credits goes to Michael Barr for the majority of this code.

 **********************************************************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <chrono>
#include <assert.h>

#include "crc.h"

using namespace std;
using namespace std::chrono;

int main(void)
{
	unsigned char test[] = "123456789";
	high_resolution_clock::time_point start, end;
	duration<double> time_span;

	/*
	 * Print the check value for the selected CRC algorithm.
	 */
	printf("The check value for the %s standard is 0x%X\n\n", CRC_NAME, CHECK_VALUE);

	/*
	 * Compute the CRC of the test message, slowly.
	 */

	start = high_resolution_clock::now();
	printf("The crcSlow() of \"123456789\" is 0x%X\n", crcSlow(test, sizeof(test)));
	end = high_resolution_clock::now();
	assert(crcSlow(test, sizeof(test)) == ASSERT_VALUE);	//unit test
	time_span = duration_cast<duration<double>>(end - start);

	cout << "The crcSlow() function took " << time_span.count() << " seconds.";
	cout << endl << endl;

	/*
	 * Compute the CRC of the test message, more efficiently.
	 */
	crcInit();
	start = high_resolution_clock::now();
	printf("The crcFast() of \"123456789\" is 0x%X\n", crcFast(test, sizeof(test)));
	end = high_resolution_clock::now();
	assert(crcSlow(test, sizeof(test)) == ASSERT_VALUE); //unit test
	time_span = duration_cast<duration<double>>(end - start);

	cout << "The crcFast() function took " << time_span.count() << " seconds.";
	cout << endl << endl;
	
}   /* main() */