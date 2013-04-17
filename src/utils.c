#include "utils.h"

unsigned char float32toubyte8(float color){


	if(color>1)
		color = 1;
	else if (color<0)
		color = 0;


	return color*255;
}