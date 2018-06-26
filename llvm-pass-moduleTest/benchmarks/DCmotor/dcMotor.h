// TODO: generate the num satate macros automatically

//#define NUM_STATES ()
//#define NUM_OUTPUTS ()
//#define NUM_INPUTS ()
//#define NUM_X ()

typedef struct{
    double voltage;
}RETURN_VAL;

typedef struct{
    double angVal;
    double  error_i_previous;
}INPUT_VAL;

RETURN_VAL* controller(INPUT_VAL* iv, RETURN_VAL* rv);

