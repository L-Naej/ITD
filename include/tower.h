#ifndef TOWERS
#define TOWERS
#include <stdlib.h>
#include "utils.h"
#include "point3D.h"
static const int TOWER_WIDTH = 30;
static const int TOWER_HEIGHT = 50;

typedef enum {
	ROCKET, LASER, GUN, HYBRID
}TowerType; 

typedef struct {
	TowerType type;
	int rate;
	int nbTurnsSinceLastShoot;
	int range;
	int power;
	float price;

	Point3D position;
}Tower;

/**
 * Retourne une tour du type spécifié en paramètre.
 **/
Tower* createTower(TowerType type);

/**
 * Fonctions appelées par createTower.
 **/
Tower* createRocketTower();
Tower* createLaserTower();
Tower* createGunTower();
Tower* createHybridTower();
#endif
