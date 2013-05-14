#include "world.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include "geometry.h"
#include "point3D.h"

//-------------- FONCTIONS PUBLIQUES --------------------------
World initWorld(const char* pathToItdFile){
	World newWorld;
	
	newWorld.worldTime = SDL_GetTicks();
	newWorld.currentMonstersWave = 0;
	newWorld.isBetweenWaves = true;
	newWorld.nbTurnsWaiting = 0;
	newWorld.nbMonstersAlive = 0;//Pas de monstres au départ
	
	newWorld.towersList = createEmptyList();
	if(newWorld.towersList == NULL){
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
	printf("Starts monster wave n°%d\n", world->currentMonstersWave);
	
	int i = 0;
	Point3D startPoint = getStartPoint(&(world->map));
	Point3D secondPoint = nextNode(world->map.pathNodeList, startPoint); 
	Vector3D direction = MultVector(Normalize(Vector(startPoint, secondPoint)), -1.0);

	//On fait démarrer les monstres "à la queuleuleu" en dehors de la map
	//Avec comme destination le point de départ de leur chemin.
	for(i = 0; i < MONSTERS_PER_WAVE; ++i){
		world->monsters[i] = createMonster(world->currentMonstersWave, i);
		world->monsters[i].destination = startPoint;
		world->monsters[i].position = PointPlusVector(startPoint, MultVector(direction, (MONSTER_WIDTH_PX+SPACE_BETWEEN_MONSTERS_PX)*i));
	}
	world->nbMonstersAlive = MONSTERS_PER_WAVE;
}

bool worldNewStep(World* world){
	if(world == NULL) return false;
	int i = 0;
	bool isGameFinished = false;
	Uint32 now = SDL_GetTicks();
	//Tps écoulé depuis le dernier tour de jeu permet de savoir combien de tour jouer cette fois
	int turnsRemaining = (now - world->worldTime) / ((int)TIMESTEP_MILLISECONDS);
	if(turnsRemaining > 0) world->worldTime = SDL_GetTicks();
	
	while(i < turnsRemaining && !isGameFinished){
		isGameFinished = doTurn(world);
		i++;
	}

	return isGameFinished;
}

bool canIPutATowerHere(World* world, int posX, int posY){
	
	Uint32 time = SDL_GetTicks();
	int tower_center = world->map.tabXYConstruct[posX][posY];
	int tower_top_right = world->map.tabXYConstruct[posX + TOWER_WIDTH/2][posY + TOWER_HEIGHT/2];
	int tower_top_left = world->map.tabXYConstruct[posX + TOWER_WIDTH/2][posY - TOWER_HEIGHT/2];
	int tower_bot_right = world->map.tabXYConstruct[posX - TOWER_WIDTH/2][posY + TOWER_HEIGHT/2];
	int tower_bot_left = world->map.tabXYConstruct[posX - TOWER_WIDTH/2][posY - TOWER_HEIGHT/2];
	if(world == NULL) return false;
	/* Tester si le centre est sur la map */
	if (posX >= world->map.width || posY >= world->map.height) return false;
	/* Tester que les tours ne débordent pas de la map */
	if ( tower_top_right > world->map.width && tower_top_right > world->map.height) return false;
	if ( tower_top_left > world->map.width && tower_top_right > world->map.height) return false;
	if ( tower_bot_right > world->map.width && tower_top_right > world->map.height) return false;
	if ( tower_bot_left > world->map.width && tower_top_right > world->map.height) return false;
	/* Tester si les coins de la tour et le centre sont constructibles */
	if ( tower_center == true && tower_top_right == true && tower_top_left == true && tower_bot_right == true && tower_bot_left == true ) {
			time = SDL_GetTicks() - time;		
			printf("Zone constructible temps de recherche : %d \n", time);
			return true;	
	}
					
	/* Tester s'il n'y a pas déjà une tour */
	
	/* Tester si le chemin ne passe pas par là */

	time = SDL_GetTicks() - time;
	printf("Zone non constructible, temps de recherche : %d \n", time);
	return false;
}

/*void addTowerOnMap(World* world, int posX, int posY, TowerType type){
	if(world == NULL || world->towersList == NULL) return;
	//Si la zone où l'on veut construire la tour est non constructible, 
	//on annule l'action.
	if(canIPutATowerHere(world, posX, posY) == false) return;
	
	Point3D towerPosition = PointXYZ(posX,posY,0);
	//L'erreur d'allocation est gérée plus bas
	//Le programme s'arrête si le malloc a échoué 
	Tower* newTower = createTower(type);
	newTower->position = towerPosition;
	
	insertBottomCell(world->towersList, newTower);
}*/


//---------------------- FONCTIONS PRIVEES ---------------------
bool doTurn(World* world){
	if(world == NULL) return false;
	static int turnNumber = 0; //debug
	turnNumber++;
	
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
	
	moveMonsters(world->monsters, world->map.pathNodeList);
	towersShoot(world->towersList, world->monsters);
	
	//Les monstres sont-ils tous morts ? 
	//Un monstre a-t-il atteint l'arrivée ?
	//Tous les monstres sont morts et c'était la dernier vague => Le jeu est fini ?
	int i = 0;
	Point3D endPoint = getEndPoint(&(world->map));
	while(i < MONSTERS_PER_WAVE && !isGameFinished){
		isGameFinished = arePointsEquals(world->monsters[i].position, endPoint);
		if(world->monsters[i].life <= 0) world->nbMonstersAlive--;
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
		
		moveMonster(&(monsters[i]));
		
		//Si on est sur un pathnode, on change de pathnode de destination
		if(arePointsEquals(monsters[i].position, monsters[i].destination)){
			monsters[i].destination = nextNode(pathNodeList, monsters[i].destination);
		}
		monsterMove = false;
	}
	
}

void towersShoot(List* towersList, Monster* monsters){
	if(towersList == NULL || monsters == NULL) return;

	Tower* curTower = NULL;	
	goToHeadList(towersList);
	
	//Pour chaque tour on regarde si elle peut tirer sur un monstre
	while( (curTower = (Tower*) nextData(towersList)) != NULL){
		curTower->nbTurnsSinceLastShoot++;
		towerShoots(curTower, monsters);
	}
}

void towerShoots(Tower* tower, Monster* monsters){
	if(tower == NULL || monsters == NULL) return;
	if(tower->rate < tower->nbTurnsSinceLastShoot) return;
	bool towerCanShoot = true;
	int i; int lifeLosed = 0;
	while(towerCanShoot && i < MONSTERS_PER_WAVE){ 
		if(monsters[i].life > 0
		&& Norm(Vector(monsters[i].position, tower->position)) <= tower->range){
			switch(tower->type){
				case ROCKET : lifeLosed = tower->power - monsters[i].rocketResistance;
				break;
				case GUN : lifeLosed = tower->power - monsters[i].gunResistance;
				break;
				case LASER : lifeLosed = tower->power - monsters[i].laserResistance;
				break;
				case HYBRID : lifeLosed = tower->power - monsters[i].hybridResistance;
				break;
				default : lifeLosed = tower->power;
				break;
			}
			monsters[i].life -= lifeLosed;
			towerCanShoot = tower->type == GUN;//Seul les GUN peuvent tirer sur tous les monstres en même temps
			tower->nbTurnsSinceLastShoot = 0;
		}
		i++;
	}
}


