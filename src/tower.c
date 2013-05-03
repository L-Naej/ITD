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

Tower* createTower(TowerType type){
	switch(type){
	case ROCKET : return createRocketTower();
	break;
	case LASER : return createLaserTower();
	break;
	case GUN : return createGunTower();
	break;
	case HYBRID : return createHybridTower();
	break;
	default : return createRocketTower();
	}
}

//TODO
Tower* createRocketTower(){
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL){
		fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire pour créer une tour.\n");
		exit(-1);
	}
	
	newTower->nbTurnsSinceLastShoot = 0;
	newTower->rate = 100;
	newTower->range = 50;
	newTower->power = 10;
	newTower->price = 100;

	newTower->position = PointXYZ(-1,-1,0);
	return newTower;
}
//TODO
Tower* createLaserTower(){
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL){
		fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire pour créer une tour.\n");
		exit(-1);
	}
	
	newTower->nbTurnsSinceLastShoot = 0;
	newTower->rate = 100;
	newTower->range = 50;
	newTower->power = 10;
	newTower->price = 100;

	newTower->position = PointXYZ(-1,-1,0);
	return newTower;
}
//TODO
Tower* createGunTower(){
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL){
		fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire pour créer une tour.\n");
		exit(-1);
	}
	
	newTower->nbTurnsSinceLastShoot = 0;
	newTower->rate = 100;
	newTower->range = 50;
	newTower->power = 10;
	newTower->price = 100;

	newTower->position = PointXYZ(-1,-1,0);
	return newTower;
}
//TODO
Tower* createHybridTower(){
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL){
		fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire pour créer une tour.\n");
		exit(-1);
	}
	
	newTower->nbTurnsSinceLastShoot = 0;
	newTower->rate = 100;
	newTower->range = 50;
	newTower->power = 10;
	newTower->price = 100;

	newTower->position = PointXYZ(-1,-1,0);
	return newTower;
}
			
