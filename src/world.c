#include "World.h"
#include <SDL/SDL.h>
#include <math.h>

World initWorld(const char* pathToItdFile){
	World newWorld;
	
	newWorld.worldTime = 0;
	newWorld.currentMonstersWave = 0;
	newWorld.isBetweenWaves = true;
	newWorld.nbTurnsWaiting = 0;
	newWorld.nbMonstersAlive = 0;//Pas de monstres au départ
	
	newWorld.currentTowersList = createEmptyList();
	if(newWorld.currentTowersList == NULL){
		perror("Erreur fatale, impossible d'allouer l'espace mémoire nécessaire.\n");
		exit(-1);
	}
	
	newWorld.map = initMap();
	loadMap(&(newWorld.map), pathToItdFile);
	
	return newWorld;
}

void startNewMonsterWave(World* world){
	if(world == NULL) return;
	world->currentMonstersWave++;
	
	int i = 0;
	for(i = 0, i < MONSTERS_PER_WAVE; ++i){
		world->monsters[i] = createMonster(world->currentMonstersWave);
	}
	world->nbMonstersAlive = MONSTERS_PER_WAVE;
}

bool worldNewStep(World* world){
	if(world == NULL) return false;
	int i = 0;
	bool isGameFinished = false;
	Uint32 now = SDL_GetTicks();
	//Tps écoulé depuis le dernier tour de jeu permet de savoir combien de tour jouer cette fois
	int turnsRemaining = (int) floor((now - world->worldTime) / (float)TIMESTEP_MILLISECONDS);
	//if (turnsRemaining == 0) turnsRemaining = 1;//à voir
	
	while(i < turnsRemaining && !isGameFinished){
		isGameFinished = doTurn(world);
		i++;
	}
	
	world->worldTime = SDL_GetTicks();
	return isGameFinished;
}

bool doTurn(World* world){
	//TODO
	if(world == NULL) return false;
	bool isGameFinished = false;
	
	//Phase d'attente entre deux vague de monstres ? On ne fait rien
	if(world->isBetweenWaves){
		world->nbTurnsWaiting++;
		if(world->nbTurnsWaiting >= NB_TURNS_BETWEEN_WAVES){
			startNewMonsterWave(world);
			world->isBetweenWaves = false;
			world->nbTurnsWaiting = 0;
		}
		return isGameFinished;
	}
	
	moveMonsters(world->currentMonsters, world->map.pathNodeList);
	towersShoot(&(world->currentTowersList));
	
	//Les monstres sont-ils tous morts ? 
	//Un monstre a-t-il atteint l'arrivée ?
	//Tous les monstres sont morts et c'était la dernier vague => Le jeu est fini ?
	int i = 0;
	while(i < MONSTERS_PER_WAVE && !isGameFinished){
		isGameFinished = arePointsEquals(world->currentMonsters[i].position,world->map.endPoint);
		if(monsters[i]->life <= 0) world->nbMonstersAlive--;
		++i;
	}
	if(world->nbMonstersAlive <= 0){
		if(world->currentMonstersWave >= NB_TOTAL_WAVES){
			isGameFinished = true;
		}
		world->isBetweenWaves = true;
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
		if(monsters[i].life <= 0) continue;
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
