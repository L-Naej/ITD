#ifndef TOWERS
#define TOWERS
#include <stdlib.h>
#include "utils.h"
#include "point3D.h"

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

typedef struct {
	int powerR;
	int rateR;
	int rangeR;
	int costR;
	int powerL;
	int rateL;
	int rangeL;
	int costL;
	int powerM;
	int rateM;
	int rangeM;
	int costM;
	int powerH;
	int rateH;
	int rangeH;
	int costH;
}TowerStat;

/**
 * initisalise une structure TowerStat qui permet d'avoir les données des différents types de tour.
 **/
TowerStat* createEmptyTowerStat();

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
