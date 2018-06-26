// Must include controller.h
#include "controller.h"

#ifdef DEBUG
  #include<stdio.h>
#endif

//void controller_init(){
//}

void* controller(double room_temp, RETURN_VAL* ret_val)
{

  static int chatter_detect;
  static int previous_command_to_heater;
  static int on_counter, off_counter;
  static int command_to_heater;
  int chatter_limit=2;
  double MAX_TEMP=70.0, MED_TEMP=66.0;

 /* room_temp =                   input->x_arr[0];
  chatter_detect =              input->int_state_arr[0];
  previous_command_to_heater =  input->int_state_arr[1];
  on_counter =                  input->int_state_arr[2];
  off_counter =                 input->int_state_arr[3];
*/
//	int NO_HEAT=0, NORMAL_HEAT = 2, FAST_HEAT = 1;

  if(room_temp >= MED_TEMP && room_temp < MAX_TEMP)
   command_to_heater = 2;
  else if(room_temp >= MAX_TEMP)
   command_to_heater = 0;
  else if(room_temp < MED_TEMP)
	command_to_heater = 1;
  else
   command_to_heater = previous_command_to_heater;
  
  if(off_counter >= 5 || on_counter >= 5)
    chatter_detect = 0;

  if(command_to_heater != previous_command_to_heater)
    chatter_detect++;

  if(chatter_detect > chatter_limit)
    command_to_heater = previous_command_to_heater;

  if(command_to_heater == 0) {
    on_counter = 0;
    off_counter++;
  } else {
    on_counter++;
    off_counter = 0;
  }

  ret_val->output_arr[0] = (double)command_to_heater;
  ret_val->int_state_arr[0] = chatter_detect;
  ret_val->int_state_arr[1] = command_to_heater;
  ret_val->int_state_arr[2] = on_counter;
  ret_val->int_state_arr[3] = off_counter;

  return (void*)0;
}
/*int main(){
	INPUT_VAL* input;
	RETURN_VAL* ret_val;

	input->x_arr[0] = 56;
	input->int_state_arr[0] = 0;
	input->int_state_arr[1] = 0;
	input->int_state_arr[2] = 0;
	input->int_state_arr[3] = 0;
	controller(input, ret_val);

	return 0;		
}*/
