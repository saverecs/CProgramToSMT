// Must include controller.h
#include "thermostat.h"

#ifdef DEBUG
  #include<stdio.h>
#endif

RETURN_VAL* thermostat(INPUT_VAL* input, RETURN_VAL* ret_val)
{
	static int chatter_detect;
	static int previous_command_to_heater;
	static int on_counter, off_counter;
	static int command_to_heater;
	int chatter_limit=2;
	double u, room_temp;
	double MAX_TEMP=70.0, MED_TEMP=66.0;

//	int NO_HEAT=0, NORMAL_HEAT = 2, FAST_HEAT = 1;

 room_temp = input->temperature;

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

	if (command_to_heater==0) 
		u = 20;
	else if (command_to_heater==1) 
		u = 100;
	else if (command_to_heater==2) 
		u = 70;

	ret_val->u = u;

  return ret_val;
}

