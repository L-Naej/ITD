#ifndef ITD_MONSTER_H
#define ITD_MONSTER_H
#include <stdlib.h>
#include "utils.h"
#include "point3D.h"
#include "vector3D.h"
#include "list.h"

/* Largeur d'un sprite de monstre en pixels (ca respecte pas trop 
mvc de le mettre là mais en attendant de trouver mieux... */
static const int MONSTER_WIDTH_PX = 35;
static const int SPACE_BETWEEN_MONSTERS_PX = 5;
typedef enum{
	BLUE_OCTOPUS,
	ORANGE_OCTOPUS,
	GREEN_OCTOPUS
}MonsterType;

typedef struct{
	MonsterType type;
	int life; 		/*points de vie*/
	int maxLife;
	int rocketResistance;
	int laserResistance;
	int gunResistance;
	int hybridResistance; 	 	/* résistance */
	int money; 
	
	//Exprimée en tours de jeux pour avancer d'un pixel
	int speed;
	
	Point3D position;
	Point3D realPosition;
	Point3D destination;
	Vector3D direction;
	
	//Nombre de tours depuis le dernier mouvement
	int nbTurnsSinceLastMove;
}Monster;

/**
 * Fait bouger le monstre d'un pixel sur la carte selon
 * sa position courante et sa destination.
 **/
void moveMonster(Monster* monster);

/**
 * Créé un monstre avec les caractéristiques correspondant
 * à la vague (de 1 à 20) à laquelle il appartient.
 * Renvoie automatiquement le bon type de monstre
 * en fonction de la vague en cours et du nombre de monstre
 * déjà créé pour la vague en cours.
 **/ 
Monster createMonster(unsigned char wave, int nbMonstersCreated);

//---------- FONCTIONS INTERNES --------------
Monster createBlueOctopus(unsigned char wave);
Monster createOrangeOctopus(unsigned char wave);
Monster createGreenOctopus(unsigned char wave);

#endif
