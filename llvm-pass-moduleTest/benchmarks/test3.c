#include <stdio.h>
void test(int x, int y){
	int z;
	//x=4;
	//y=6;
	if (x >= y)
		x++;
	else if (x <= 3)
		y++;	
	else
		y=77;
	z=x+y;
	x = x + y;

}
