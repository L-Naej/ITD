#ifndef ITD_MONSTER_H
#define ITD_MONSTER_H
#include <stdlib.h>
#include "utils.h"
#include "point3D.h"
#include "list.h"

typedef enum {
	MONSTER_TYPE1, MONSTER_TYPE2 
}MonsterType;

typedef struct{
	MonsterType type; 	/* type de monster */	
	int life; 		/*points de vie*/
	int strenght; 	 	/* résistance */
	int money; 
	
	//Exprimée en tours de jeux pour avancer d'un pixel
	unsigned char speed;
	
	Point3D position;
	Point3D destination;
	
	//Nombre de tours depuis le dernier mouvement
	unsigned char nbTurnsSinceLastMove;

	List* myWay;
}

/**
 * Fait bouger le monstre d'un pixel sur la carte selon
 * sa position courante et sa destination.
 **/
void moveMonster(Monster* monster);

/**
 * Créé un monstre avec les caractéristiques correspondant
 * à la vague (de 1 à 20) à laquelle il appartient.
 **/ 
Monster createMonster(unsigned int wave);

Monster createMonster1();
Monster createMonster2();

#endif
