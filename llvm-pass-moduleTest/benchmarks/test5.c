#include <stdio.h>
int controller(int x, int y){
	int i,z=0;
	for (i=0;i<4;i++){
		z = z + y + i + x;	
	}
	return z;
}
