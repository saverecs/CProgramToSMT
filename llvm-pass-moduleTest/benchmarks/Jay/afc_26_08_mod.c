#include "controller.h"

//typedef unsigned char boolean_T;

typedef unsigned int boolean_T;

//Constants:
  #define Constant2_Value 1.0
  #define fb_fuel_saturation_UpperSat 100.0
  #define fb_fuel_saturation_LowerSat 0
  #define Constant3_Value 1
  #define Constant1_Value 0.01
  #define c2  -0.366
  #define c3  0.08979 
  #define c4 -0.0337
  #define c5 0.0001 
  #define UnitDelay1_InitialCondition 0.982
  #define c1 0.41328
  #define Constant1_Value_m 0.01 
  #define c13 0.04
  #define c14 0.14
  #define UnitDelay1_InitialCondition_f 0.0 
  #define fuel_saturation_UpperSat 1.66 
  #define fuel_saturation_LowerSat 0.13 
  #define c11 14.7 
  #define c11p 12.5 
  #define UnitDelay2_InitialCondition 0.0 
  #define sampling_sec_Value  0.01 
  #define normal_mode_start_sec_Value  10.0 
  #define DataStoreMemory_InitialValue  0
  #define DataStoreMemory1_InitialValue  0
  #define DataStoreMemory2_InitialValue  0
  #define DataStoreMemory3_InitialValue  0
  #define commanded_fuel_InitialValue  0.1726 
  #define mode_fb1_InitialValue  14.7 
  #define UnitDelay1_InitialCondition_l  0 
  #define UnitDelay1_InitialCondition_j  0
  #define UnitDelay_InitialCondition  0
  #define mode_fb_InitialValue  1 

void* controller(INPUT_VAL* input, RETURN_VAL* ret_val)
{

	// plant states 	(Sensed)
	double temp_engine_speed = input->state_p_engine_speed;
	double temp_throttle_angle = input->state_p_throttle_angle;
	double temp_throttle_flow = input->state_p_throttle_flow;
	double temp_airbyfuel_meas = input->state_p_airbyfuel_meas;

	// Control Variables
	double prev_pi = input->state_p_pi;
	double prev_air_estimate = input->state_p_air_estimate;
	double pi;
	double air_estimate;

	//Intermediate Program variables
	double normal_mode_detect_1 ;
//	boolean_T sensor_fail_detect;
	boolean_T power_mode_detect ;
	boolean_T normal_mode_detect_1_e;
	boolean_T rtb_LogicalOperator;
	boolean_T rtb_LogicalOperator_d;
	double rtb_Sum_d;
	double rtb_Sum1_d;
	boolean_T rtb_RelationalOperator1;
	double rtb_Sum3;
	boolean_T rtb_DataStoreRead3_f;
	double rtb_Sum2;
	double Sum3;
	double rtb_Switch_n0;
	double rtb_DataStoreRead;

	// output(s)
	double commanded_fuel_gps;



	//Detect Sensor failure
	if (temp_airbyfuel_meas <= -1.0)
		rtb_LogicalOperator = 1;
	else
		rtb_LogicalOperator = 0;
	//Condition check for Normal Mode

	//Jay(Added below line): Analyzing S/W for Normal Mode of Operation
	normal_mode_detect_1_e = 1;

//	rtb_LogicalOperator_d = 1;

	rtb_Sum_d = normal_mode_detect_1 + sampling_sec_Value;
	rtb_LogicalOperator_d = ((rtb_Sum_d >= normal_mode_start_sec_Value) || normal_mode_detect_1_e);
	normal_mode_detect_1 = rtb_Sum_d;
	normal_mode_detect_1_e = rtb_LogicalOperator_d;

	if (power_mode_detect) {
		rtb_Sum1_d = 50.0;
	} else {
		rtb_Sum1_d = 50.0 + 20.0;
	}

	//Decide Reference value for labda
	rtb_RelationalOperator1 = (temp_throttle_angle >= rtb_Sum1_d);
	power_mode_detect = rtb_RelationalOperator1;
	rtb_LogicalOperator = (rtb_LogicalOperator || (!rtb_LogicalOperator_d) || rtb_RelationalOperator1);

	if (rtb_LogicalOperator_d && rtb_RelationalOperator1) {
		rtb_Sum_d = c11p;
	} else {
		rtb_Sum_d = c11;
	}

		rtb_Sum_d = c11;

	//Calculate P_estimate
	rtb_Sum3 = ((prev_air_estimate * temp_engine_speed * c3 + c2) + prev_air_estimate * prev_air_estimate * temp_engine_speed * c4) + temp_engine_speed * temp_engine_speed * prev_air_estimate * c5;

	air_estimate = prev_air_estimate + (temp_throttle_flow - rtb_Sum3) * c1 * Constant1_Value;

	//rtb_DataStoreRead3_f = rtb_LogicalOperator;
//	rtb_DataStoreRead3_f = 0;

	if (!rtb_DataStoreRead3_f) {
		//Calculate Pi for Normal/Power Mode Operation
		rtb_Sum1_d = temp_airbyfuel_meas - rtb_Sum_d;
		rtb_Sum2 = c14 * rtb_Sum1_d * Constant1_Value_m + prev_pi;
		Sum3 = c13 * rtb_Sum1_d + rtb_Sum2;
		pi = rtb_Sum2 ;
	}


	//Checks for Saturations
	if (rtb_DataStoreRead3_f) {
		rtb_Switch_n0 = Constant3_Value;
	} else {
		rtb_Sum1_d = Constant2_Value + Sum3;
		if (rtb_Sum1_d > fb_fuel_saturation_UpperSat) {
			rtb_Switch_n0 = fb_fuel_saturation_UpperSat;
		} else if (rtb_Sum1_d < fb_fuel_saturation_LowerSat) {
			rtb_Switch_n0 = fb_fuel_saturation_LowerSat;
		} else {
			rtb_Switch_n0 = rtb_Sum1_d;
		}
	}

	rtb_Sum1_d = rtb_Sum3 / rtb_Sum_d * rtb_Switch_n0;
	if (rtb_Sum1_d > fuel_saturation_UpperSat) {
		rtb_DataStoreRead = fuel_saturation_UpperSat;
	} else if (rtb_Sum1_d < fuel_saturation_LowerSat) {
		rtb_DataStoreRead = fuel_saturation_LowerSat;
	} else {
		rtb_DataStoreRead = rtb_Sum1_d;
	}

	commanded_fuel_gps = rtb_DataStoreRead;
	//output to plant
	ret_val->next_c_commanded_fuel_gps = commanded_fuel_gps;
	ret_val->next_p_air_estimate = air_estimate;
	ret_val->next_p_pi = pi;

	return (void*)0;
}

