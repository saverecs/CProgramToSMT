#include "controller_simple.h"
typedef double real32_T;
typedef unsigned char boolean_T;

void* controller(INPUT_VAL* input, RETURN_VAL* ret_val)
//void* main(INPUT_VAL* input, RETURN_VAL* ret_val)
{       

	// outputs
	double commanded_fuel_gps;

	// plant states 	(Sensed)
	double engine_speed = input->state_p_engine_speed;
	double throttle_angle = input->state_p_throttle_angle;
	double manifold_pressure = input->state_p_manifold_pressure;
	double airbyfuel_meas = input->state_p_airbyfuel_meas;

	//intermediate values of control program
	double pressure_est = input->p_air_estimate;
	double integral_error = input->p_pi;

	if(pressure_est == 0)
	{
		pressure_est = 0.9;
	}

	pressure_est = pressure_est + ( Sampling_Time * c1 * (c23 * dmAf - mCf(pressure_est) ) );
	integral_error = integral_error + Sampling_Time * c14 * (c24 * airbyfuel_meas - c11) ;
	commanded_fuel_gps = ((1 + integral_error + (c13 * ((c24 * airbyfuel_meas) - c11))) * mCf(pressure_est)) / c11 ;	

	//output to plant
	ret_val->next_c_commanded_fuel_gps = commanded_fuel_gps;

return (void*)0;
}

/*
void main()
{
	INPUT_VAL* dumy_input;
	RETURN_VAL* dumy_ret_val;

	dumy_input->state_p_engine_speed = 95 ;
	dumy_input->state_p_throttle_angle = 8.8 ;
	dumy_input->state_p_throttle_flow = 4.2 ;
	dumy_input->state_p_manifold_pressure = 0.9833;
	dumy_input->state_p_airbyfuel_meas = 14.7 ;


	controller(INPUT_VAL* input, RETURN_VAL* ret_val) ;

	printf ("\n\n\t Fcmd: %f \n\n", ret_val->next_c_commanded_fuel_gps);

}

*/

