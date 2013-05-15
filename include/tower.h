#ifndef TOWERS
#define TOWERS
#include <stdlib.h>
#include "utils.h"
#include "point3D.h"
#include "map.h"
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
	float cost;

	Point3D position;
}Tower;



/**
 * initisalise une structure TowerStat qui permet d'avoir les données des différents types de tour.
 **/
TowerStat* createEmptyTowerStat();

/**
 * Retourne une tour du type spécifié en paramètre.
 **/
Tower* createTower(TowerType type,Map map);

/**
 * Fonctions appelées par createTower.
 **/
Tower* createRocketTower(Map map);
Tower* createLaserTower(Map map);
Tower* createGunTower(Map map);
Tower* createHybridTower(Map map);
#endif
