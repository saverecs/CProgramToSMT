#include <stdio.h>


typedef double real32_T;
typedef unsigned char boolean_T;

int MAX = 170;	//NOTE THIS WILL NOT GET PROPAGATED

int controller(int x, int y){
	int i,z=0;

//int MAX = 170;	//NOTE THIS WILL BE PROPAGATED

struct P_Z_AF_Controller_T_ {
  real32_T Constant2_Value;
  real32_T fb_fuel_saturation_UpperSat;  
  boolean_T mode_fb_InitialValue;
};
typedef struct P_Z_AF_Controller_T_ P_Z_AF_Controller_T;
P_Z_AF_Controller_T Z_AF_Controller_P = {
  1.0,
  100.0,
  1
};

i = ((x <= Z_AF_Controller_P.fb_fuel_saturation_UpperSat) || y);
  z = i + MAX;
  
	return z;
}
