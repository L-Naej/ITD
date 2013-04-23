#include "World.h"
#include <SDL/SDL.h>
#include "utils.h"

bool worldNewStep(World* world){
	if(world == NULL) return false;
	
	int i = 0;
	bool isGameFinished = false;
	Uint32 now = SDL_GetTicks();
	int turnsRemaining = (now - world->worldTime) / TIMESTEP_MILLISECONDS;
	if (turnsRemaining == 0) turnsRemaining = 1;
	
	while(i < turnsRemaining && !isGameFinished){
		isGameFinished = doTurn(world);
		i++;
	}
	return isGameFinished;
}

bool doTurn(World* world){
//TODO
	if(world == NULL) return false;
	bool isGameFinished = false;
	
	moveMonsters(world->currentMonsters, world->loadedMap.nodeList);
	towersShoot(&(world->currentTowersList));
	
	//Le jeu est-il fini ?
	//On regarde si un monstre a atteint l'arrivée
	int i = 0;
	while(i < MONSTERS_PER_WAVE && !isGameFinished){
		isGameFinished = arePointsEquals(world->currentMonsters[i].position,world->loadedMap.endPoint);
		++i;
	}
	
	return isGameFinished;
}

//Note : la variable monster->speed est égale au nombre de tours nécessaires au monstre pour
//avancer d'un pixel.
void moveMonsters(Monster* monsters, List* pathNodeList){
	if(monsters == NULL) return;
		
	int i = 0;
	bool monsterMove = false;
	for(i = 0; i < MONSTERS_PER_WAVE; ++i){
		monsters[i].nbTurnsSinceLastMove++;
		
		monsterMove = (monsters[i].nbTurnsSinceLastMove /  monsters[i].speed) >= 1;
		if(monsterMove) moveMonster(&(monsters[i]));
		
		//Si on est sur un pathnode, on change de pathnode de destination
		if(arePointsEquals(monsters[i].position, monsters[i].destination)){
			monsters[i].destination = nextNode(pathNodeList, monsters[i].destination);
		}
		
		monsters[i].nbTurnsSinceLastMove = 0;
		monsterMove = false;
	}
	
}

void towersShoot(List* towerList){}
