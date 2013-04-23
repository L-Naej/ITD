#ifndef ITD_MONSTER_H
#define ITD_MONSTER_H
#include <stdlib.h>
#include "utils.h"
#include "point3D.h"
#include "list.h"

typedef struct{
	Point3D position;
	Point3D destination;
	
	//Nombre de tours depuis le dernier mouvement
	unsigned char nbTurnsSinceLastMove;
	unsigned char healthPoints;
	//Exprimée en tours de jeux pour avancer d'un pixel
	unsigned char speed;
	
	List* myWay;
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

/*
 * Fait bouger le monstre d'un pixel sur la carte selon
 * sa position courante et sa destination.
 */
void moveMonster(Monster* monster);
Monster createMonster1();
Monster createMonster2();
Monster createMonster();

#endif
