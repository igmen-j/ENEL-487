# ENEL 487 Project

This project simulates a traffic light controller with RTOS.  
It uses a nucleo-64 f103rb microcontroller board as well as eight LEDS.  
[Project Requirements](https://github.com/naqvik/ItC/blob/master/docs/requirements.md)

## Commands (description taken from the link above)
__help__: display a list of valid commands  
__mode fsm__: switch to Failsafe mode  
__mode scm__: switch to Static Cycle mode  
__atm x__: enter accelerated test mode with multiplication factor x (1-100)  

## Tasks (high to low priority)
__State_Controller_Task__: controls the state of the LEDs and their period  
__CLI_Task__: handles the CLI and builds the user command  
__Status_Update_Task__: displays the status of the system to the CLI

## Queues
__CLI_Queue__: used for _CLI_Task_ and _State_Controller_Task_ communication  
__Status_Queue__: used for _Status_Update_Task_ and _State_Controller_Task_ communication  

## Issues/Limitations
- One full cycle of the lights has to run before the mode switches (blocking)  
  - This is noticeable when switching from SCM to FSM modes&mdash;especially at x1 speed  
- Gets a Hard Fault when user enters characters over the limit to transmit (max input: 50)
