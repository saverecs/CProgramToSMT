typedef struct{
	
	double c_commanded_fuel_gps;
	double c_controller_mode;
	double c_airbyfuel_ref;

}RETURN_VAL;

typedef struct{

	double p_normal_mode_detect_1;
	double p_pi;
	double p_air_estimate;
	double p_sensor_fail_detect;
	double p_power_mode_detect;
	double p_normal_mode_detect_1_e;
	double p_Sum3;

   double p_engine_speed;
   double p_throttle_angle;
   double p_throttle_flow;
   double p_airbyfuel_meas;

}INPUT_VAL;

void* controller(INPUT_VAL* iv, RETURN_VAL* rv);

