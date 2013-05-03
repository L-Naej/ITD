#include "tower.h"
#include <stdio.h>

/* les données sont pour les fichier ITD1, elles sont modifiées dans loadMap pour les ITD2 */
TowerStat* createEmptyTowerStat(){
	TowerStat* towerstat = (TowerStat*) malloc(sizeof(TowerStat));
	if(towerstat == NULL) return NULL;
	towerstat->powerR=100;
	towerstat->rateR=10;
	towerstat->rangeR=50;
	towerstat->costR=60;
	towerstat->powerL=50;
	towerstat->rateL=100;
	towerstat->rangeL=10;
	towerstat->costL=40;
	towerstat->powerM=20;
	towerstat->rateM=70;
	towerstat->rangeM=20;
	towerstat->costM=30;
	towerstat->powerH=40;
	towerstat->rateH=80;
	towerstat->rangeH=100;
	towerstat->costH=90;
}

Tower* createTower(TowerType type,Map map){
	switch(type){
	case ROCKET : return createRocketTower(map);
	break;
	case LASER : return createLaserTower(map);
	break;
	case GUN : return createGunTower(map);
	break;
	case HYBRID : return createHybridTower(map);
	break;
	default : return createRocketTower(map);
	}
}

//TODO
Tower* createRocketTower(Map map){
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL){
		fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire pour créer une tour.\n");
		exit(-1);
	}
	
	newTower->nbTurnsSinceLastShoot = 0;

	newTower->power = map.towerdatas->powerR;
	newTower->rate = map.towerdatas->rateR;
	newTower->range = map.towerdatas->rangeR;
	newTower->cost = map.towerdatas->costR;


	newTower->position = PointXYZ(-1,-1,0);
	return newTower;
}
//TODO
Tower* createLaserTower(Map map){
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL){
		fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire pour créer une tour.\n");
		exit(-1);
	}
	
	newTower->nbTurnsSinceLastShoot = 0;
	newTower->power = map.towerdatas->powerL;
	newTower->rate = map.towerdatas->rateL;
	newTower->range = map.towerdatas->rangeL;
	newTower->cost = map.towerdatas->costL;

	newTower->position = PointXYZ(-1,-1,0);
	return newTower;
}
//TODO
Tower* createGunTower(Map map){
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL){
		fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire pour créer une tour.\n");
		exit(-1);
	}
	
	newTower->nbTurnsSinceLastShoot = 0;
	newTower->power = map.towerdatas->powerM;
	newTower->rate = map.towerdatas->rateM;
	newTower->range = map.towerdatas->rangeM;
	newTower->cost = map.towerdatas->costM;

	newTower->position = PointXYZ(-1,-1,0);
	return newTower;
}
//TODO
Tower* createHybridTower(Map map){
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL){
		fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire pour créer une tour.\n");
		exit(-1);
	}
	
	newTower->nbTurnsSinceLastShoot = 0;
	newTower->power = map.towerdatas->powerH;
	newTower->rate = map.towerdatas->rateH;
	newTower->range = map.towerdatas->rangeH;
	newTower->cost = map.towerdatas->costH;

	newTower->position = PointXYZ(-1,-1,0);
	return newTower;
}
			
