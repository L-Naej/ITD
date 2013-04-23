#ifndef LDR_MONSTER
#define LDR_MONSTER

typedef enum {
	MONSTER_TYPE1, MONSTER_TYPE2 
}MonsterType;

typedef struct monster {
	MonsterType type; 	/* type de monster */	
	int life; 		/*points de vie*/
	int strenght; 	 	/* résistance */
	int money; 		/* unité d'argent */
	float speed; 		/* vitesse */
	float posX;		/* position x */
	float posY;		/* position y */
}Monster;

Monster createMonster1();
Monster createMonster2();
Monster createMonster();

#endif
