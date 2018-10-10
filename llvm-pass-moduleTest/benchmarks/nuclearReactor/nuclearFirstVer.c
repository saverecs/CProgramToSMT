// Must include controller.h
#include "nuclear.h"

#ifdef DEBUG
  #include<stdio.h>
#endif

#define VAL1  510
#define VAL2  550

void* controller(INPUT_VAL* input, RETURN_VAL* ret_val)
{
	int out_r1, out_r2;
	double out_c1, out_c2;
	double temp;
	double count_r1;
	double count_r2;
	int in_state_r1;
	int in_state_r2;
 	int out_shut_down;	

	temp = input->temperature;
	count_r1 = input->c1;
	count_r2 = input->c2;
	in_state_r1 = input->state_r1;
	in_state_r2 = input->state_r2;

	out_shut_down = input->shutdown;


	out_c1 = count_r1;
	out_c2 = count_r2;

	if ((temp <=  VAL2) && (in_state_r1==0) && (in_state_r2==0)) {
			out_r1 = 0;
			out_r2 = 0;
	} else if ((temp >=  VAL2) && (count_r1>=20)) {
			out_r1 = 1;
			out_r2 = 0;
//			out_c1 = count_r1;
//			out_c2 = count_r2;
	} else if ((temp >=  VAL2) && (count_r2>=20)) {
			out_r1 = 0;
			out_r2 = 1;
//			out_c1 = count_r1;
//			out_c2 = count_r2;
	} else if ((temp >=  VAL2) && (count_r1<20) && (count_r2<20)) {
			out_shut_down = 1;

			out_r1 = 0;
			out_r2 = 1;
//			out_c1 = count_r1;
//			out_c2 = count_r2;
	} else if ((temp <=  VAL1) && (in_state_r1==1)) {
			out_r1 = 0;
			out_r2 = 0;
			out_c1 = 0;
//			out_c2 = count_r2;
	} else if ((temp <=  VAL1) && (in_state_r2==1)) {
			out_r1 = 0;
			out_r2 = 0;
//			out_c1 = count_r1;
			out_c2 = 0;
	} else if ((temp >=  VAL1) && (in_state_r1==1)) {
			out_r1 = 1;
			out_r2 = 0;
//			out_c1 = count_r1;
//			out_c2 = count_r2;
	} else if ((temp >=  VAL1) && (in_state_r2==1)) {
			out_r1 = 0;
			out_r2 = 1;
//			out_c1 = count_r1;
//			out_c2 = count_r2;
	}

  ret_val->r1 = out_r1;
  ret_val->r2 = out_r2;

  input->c1 = out_c1;
  input->c2 = out_c2;
  input->state_r1 = out_r1;
  input->state_r2 = out_r2;
  input->shutdown = out_shut_down;
  
  return (void*)0;
}


/*
int main(){
	INPUT_VAL* input;
	RETURN_VAL* ret_val;
	
	input->c1 = 23;
  input->c2 = 23;
  input->state_r1 = 1;
  input->state_r2 = 0;
  input->shutdown = 1;
  
	controller(input, ret_val);

	return 0;
}
*/
