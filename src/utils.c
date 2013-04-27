#include "utils.h"

int testItdValid(unsigned char R,unsigned char V,unsigned char B){
	if ((R>=0 && R<= 255) && (V>=0 && V<= 255) && (B>=0 && B<= 255)){
		return 1;
	}
	else{
		return 0;
	}

}
