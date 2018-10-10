// Must include controller.h
#include "smallBenchmark.h"

#ifdef DEBUG
  #include<stdio.h>
#endif

void* controller(INPUT_VAL* input, RETURN_VAL* ret_val)
{
	int temp_x, temp_next_u;

 temp_x = input->state_x;

	if (temp_x <= 10) 
		temp_next_u = 5;
	else if (temp_x >= 10 && temp_x <= 20) 
		temp_next_u = 15;
	else 
		temp_next_u = 25;


//	temp_next_u = 15;


	ret_val->next_u = temp_next_u;

  return ret_val;
}

