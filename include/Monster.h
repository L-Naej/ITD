#ifndef ITD_MONSTER_H
#define ITD_MONSTER_H
#include <stdlib.h>
#include "Utils.h"

typedef struct{
	float positionX;
	float positionY;
	
	//Nombre de tours depuis le dernier mouvement
	Uint32 nbTurnsSinceLastMove;
	Uint32 healthPoints;
	//Exprimée en tours de jeux pour avancer d'un pixel
	Uint32 speed;
}Monster;

#endif
