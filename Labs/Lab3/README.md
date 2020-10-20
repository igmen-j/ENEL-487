# Lab 3 - CLI 2 on the Target

This lab builds on what we learned in [Lab 2](https://github.com/igmen-j/ENEL-487/blob/master/Labs/Lab2/).  
Code: https://github.com/igmen-j/ENEL-487/blob/master/Labs/Lab3/Core/Src/main.c  
This lab, however, uses a non-blocking interrupts (ie HAL_UART_Receive_IT) and uses ANSI escape sequences

## Equipment and Software Requirements
- PC
- STM32CubeIDE
- Nucleo-64 development kit and USB cable
- Com client such as Putty

## Setup
- To setup the CLI, go to Device Manager->Ports->Note the port the STM32 is on (in my case, it is on COM3)
- Then, go to PuTTY set Serial as the connection type, enter the port on the Serial line, and the default speed  
- Run the program  

## Commands 
- help : displays the commands the user can enter  
- toggle : turns LED on or off
- state : displays the state of the LED  

## HAL commands
- HAL_UART_Transmit: The boards sends information to CLI
- HAL_UART_Receive_IT: The boards receives information from CLI (non-blocking interrupt)  
- HAL_UART_RxCpltCallback: Runs when an interrupt is received -- does the required command
- HAL_GPIO_TogglePin: Toggles the pin
- HAL_GPIO_ReadPin: Read the state of the pin (GPIO_PIN_SET/GPIO_PIN_RESET)

## Escape sequences
- \x1b[8;r : creates a scrollable command line at row 8  
- \x1b[8;0H : moves the cursor to row 8, column 0  
- \x1b[1K : clears the entire line

## How it works
- The board checks if a character is entered in the CLI
- It transmits this back to CLI to be displayed--backspace works as intended
- The entered character will be stored in a variable cmd
- This will loop until the user presses enter
- cmd will then be compared to the list of commands
- Do the specified command/print error message
