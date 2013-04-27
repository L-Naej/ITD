#include "tower.h"

#include "enum_tower.h"
#include <stdio.h>
#include <stdlib.h>


void TestTypeTower(Tower tower){
	switch (tower.type){
		case ROCKET:
			tower.rate =3;
			tower.range = 9;
			tower.power=10;
			tower.cost = 10;
		break;

		case LASER:
			tower.rate =10;
			tower.range = 3;
			tower.power=5;
			tower.cost = 6;
		break;

		case MITRAILLETTE:
			tower.rate =10;
			tower.range = 3;
			tower.power=3;
			tower.cost = 4;
		break;

		case HYBRID:
			tower.rate = 8;
			tower.range = 10;
			tower.power=3;
			tower.cost = 8;
		break;

		
		default :
			printf ("error type doesn't exist");
			return;
		break;
	}

}
