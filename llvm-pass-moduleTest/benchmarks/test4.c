#include <stdio.h>
void controller(int x, int y){
	int z;
	if (x>y){
		x++;
		if (x>100){
			x=20+y;	
			z=x+70;		
		} else {
			y=22+x;	
			z=y+77;
		}
	} else{
		x=x+2;	
		z=x+y;
	}
	x = x + y;
	z = x * y;
	y = y * 100;
}
