#ifndef TOWERS
#define TOWERS
#include "enum_tower.h"
#include "tower.h"

typedef union {
	TowerType type;
	Tower tower;
}TowerUnion;



#endif
