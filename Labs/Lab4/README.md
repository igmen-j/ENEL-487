# Lab 4- High-res Timing  

Objectives:  
1. Create a high-resolution timer driver based on the hardware timers built into the stm32f103rb.  
2. Use this timer to measure a number of basic arithmetic operations and estimate average and
worst-case execution time (WCET).  
3. Add a command to your CLI that runs your timing test suite.  

Equipment and Software Requirements:
PC  
STM32CubeIDE  
Nucleo-64 development kit and USB cable  
Com client such as Putty  

Files:  
header files  
main.c  
cli.c - handles all cli related functions  
timer.c - handles all timer related functions  
operations.c - records the time it takes for each operation and average it out over 100 iterations  

