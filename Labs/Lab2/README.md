# Lab 2 - CLI on the Target  

This lab is all about USART and how the STM32 Nucleo board communicates using the CLI.  
Code: https://github.com/igmen-j/ENEL-487/blob/master/Labs/Lab2/Core/Src/main.c

## Equipment and Software Requirements
- PC
- STM32CubeIDE
- Nucleo-64 development kit and USB cable
- Com client such as Putty

## Setup
- To setup the CLI, go to Device Manager->Ports->Note the port the STM32 is on (in my case, it is on COM6)
- Then, go to PuTTY set Serial as the connection type, enter the port on the Serial line, and the default speed  
- Run the program  

## Commands 
- help : displays the commands the user can enter  
- toggle : turns LED on or off
- state : displays the state of the LED  

## HAL commands
- HAL_UART_Transmit: The boards sends information to CLI
- HAL_UART_Receive: The boards receives information from CLI  
- HAL_GPIO_TogglePin: Toggles the pin
- HAL_GPIO_ReadPin: Read the state of the pin (GPIO_PIN_SET/GPIO_PIN_RESET)

## How it works
- The board checks if a character is entered in the CLI
- It transmits this back to CLI to be displayed--backspace works as intended
- The entered character will be stored in a variable cmd
- This will loop until the user presses enter
- cmd will then be compared to the list of commands
- Do the specified command/print error message
