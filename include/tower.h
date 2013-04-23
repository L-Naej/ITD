#ifndef TOWERS
#define TOWERS
#include "enum_tower.h"

typedef struct {
	TowerType type;
	int cadence;
	int range;
	int power;
	float price;
	float lastshot;
	float x;
	float y;
}Tower;



#endif
