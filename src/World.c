#include "World.h"
#include <SDL/SDL.h>
#include "Utils.h"

bool worldNewStep(World* world){
	if(world == NULL) return false;
	
	int i = 0;
	bool isGameFinished = false;
	Uint32 now = SDL_GetTicks();
	int turnsRemaining = (now - world->worldTime) / TIMESTEP_MILLISECONDS;
	if (turnsRemaining == 0) turnsRemaining = 1;
	
	while(i < turnsRemaining && !isGameFinished){
		isGameFinished = doTurn();
		i++;
	}
	return isGameFinished;
}

bool doTurn(World* world){
	if(world == NULL) return false;
	
	monstersMove(&(world->currentMonsters));
	towersShoot(&(world->currentTowersList));

}

void monstersMove(Monster* monsters){
	if(monsters == NULL) return;
		
	int i = 0;
	int nbPixelMove = 0;
	for(i = 0; i < MONSTERS_PER_WAVE; ++i){
		nbPixelMove = monsters[i]->speed / monsters[i]->nbTurnsSinceLastMove;
		
		//Calculer newPosX et newPosY
		
	}
	
}
