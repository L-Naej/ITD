#include "utils.h"

unsigned char int32toubyte8(int color){


	if(color>1)
		color = 1;
	else if (color<0)
		color = 0;


	return color*255;
}
