# ENEL 487 Project

This project simulates a traffic light controller with RTOS.  
It uses a nucleo-64 f103rb microcontroller board as well as eight LEDS.  
[Project Requirements](https://github.com/naqvik/ItC/blob/master/docs/requirements.md)

### Commands (description taken from the link above)
_help_: display a list of valid commands  
_mode fsm_: switch to Failsafe mode  
_mode scm_: switch to Static Cycle mode  
_atm x_: enter accelerated test mode with multiplication factor x (1-100)  

### Tasks (high to low priority)
_State_Controller_Task_: controls the state of the LEDs and their period
_CLI_Task_: handles the CLI and builds the user command  
_Status_Update_Task_: displays the status of the system to the CLI

### Queues
_CLI_Queue_: used for _CLI_Task_ and _State_Controller_Task_ communication  
_Status_Queue _: used for _Status_Update_Task_ and _State_Controller_Task_ communication  
