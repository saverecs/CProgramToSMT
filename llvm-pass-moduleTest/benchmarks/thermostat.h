
typedef struct{
    double u;
}RETURN_VAL;

typedef struct{
    double temperature;
}INPUT_VAL;

RETURN_VAL* thermostat(INPUT_VAL* iv, RETURN_VAL* rv);

