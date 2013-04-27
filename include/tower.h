#ifndef TOWERS
#define TOWERS
#include "enum_tower.h"

typedef struct {
	TowerType type;
	int rate;
	int range;
	int power;
	int cost;
	float lastshot;
	float x;
	float y;
}Tower;



#endif
