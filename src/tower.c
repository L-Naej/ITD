#include "tower.h"

#include "enum_tower.h"
#include <stdio.h>
#include <stdlib.h>


void TestTypeTower(Tower tower){
	switch (tower.type){
		case ROCKET:
			tower.cadence =3;
			tower.range = 9;
			tower.power=10;
		break;

		case LASER:
			tower.cadence =10;
			tower.range = 3;
			tower.power=5;
		break;

		case MITRAILLETTE:
			tower.cadence =10;
			tower.range = 3;
			tower.power=3;
		break;

		case HYBRID:
			tower.cadence = 8;
			tower.range = 10;
			tower.power=3;
		break;

		
		default :
			printf ("error type doesn't exist");
			return;
		break;
	}

}
