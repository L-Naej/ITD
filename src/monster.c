#include "monster.h"


Monster createMonster1() {

	/*Type monster = MONSTER_TYPE1;*/
	Monster monster;
	
	monster.life = 3;
	monster.strenght = 1;
	monster.money = 5;
	monster.speed = 0.5;
	monster.posX = 0;
	monster.posY = 0;

	return monster;
	
}


Monster createMonster2() {

	/*Type monster = MONSTER_TYPE2;*/	
	Monster monster;
	
	monster.life = 3;
	monster.strenght = 1;
	monster.money = 5;
	monster.speed = 1;
	monster.posX = 0;
	monster.posY = 0;

	return monster;
	
}

Monster createMonster(Monster monster){
	switch (monster.type){
		case MONSTER_TYPE1:
			return createMonster1();
		break;

		case MONSTER_TYPE2:
			return createMonster2();
		break;
		default :
			return createMonster1();
		break;
	}

}

