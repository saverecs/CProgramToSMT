
typedef struct{
   int r1;	//rod1 
   int r2;	//rod2 

}RETURN_VAL;

typedef struct{
   double temperature;
   double c1;	//state of rod1 if >20 only then can be inserted into the reactor
   double c2;	//state of rod2 if >20 only then can be inserted into the reactor
	
   int state_r1;	//to remember the previous state of rod1 
   int state_r2;	//to remember the previous state of rod2 
	int shutdown;
	
}INPUT_VAL;

void* controller(INPUT_VAL* iv, RETURN_VAL* rv);

