#include "tower.h"
#include "union_tower.h"
#include "enum_tower.h"
#include <stdio.h>
#include <stdlib.h>


void TestTypeTower(TowerUnion tower){
	switch (tower.type){
		case ROCKET:
			tower.tower.cadence =3;
			tower.tower.range = 9;
			tower.tower.power=10;
		break;

		case LASER:
			tower.tower.cadence =10;
			tower.tower.range = 3;
			tower.tower.power=5;
		break;

		case MITRAILLETTE:
			tower.tower.cadence =10;
			tower.tower.range = 3;
			tower.tower.power=3;
		break;

		case HYBRID:
			tower.tower.cadence = 8;
			tower.tower.range = 10;
			tower.tower.power=3;
		break;

		
		default :
			printf ("error type doesn't exist");
			return;
		break;
	}

}
