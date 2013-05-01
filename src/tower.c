#include "tower.h"
#include <stdio.h>

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
			
