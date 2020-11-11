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

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <chrono>

#include "crc.h"

using namespace std;
using namespace std::chrono;

int main(void)
{
	unsigned char test[] = "123456789";
	high_resolution_clock::time_point start, end;

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
	duration<double> time_span = duration_cast<duration<double>>(end - start);

	cout << "The crcSlow() function took " << time_span.count() << " seconds.";
	cout << endl << endl;

	/*
	 * Compute the CRC of the test message, more efficiently.
	 */
	crcInit();
	start = high_resolution_clock::now();
	printf("The crcFast() of \"123456789\" is 0x%X\n", crcFast(test, sizeof(test)));
	end = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(end - start);

	cout << "The crcFast() function took " << time_span.count() << " seconds.";
	cout << endl << endl;
	
}   /* main() */