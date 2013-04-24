#ifndef TOWERS
#define TOWERS
#include <stdlib.h>
#include "utils.h"

typedef enum {
	ROCKET, LASER, MITRAILLETTE, HYBRID
}TowerType; 

typedef struct {
	TowerType type;
	int cadence;
	int range;
	int power;
	float price;
	Uint32 lastShot;
	float positionX;
	float positionY;
#endif
