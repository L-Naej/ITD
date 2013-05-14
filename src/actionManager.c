#include "actionManager.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#ifdef MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <math.h>
#include "graphics.h"
#include "interfaceDrawer.h"

bool handleMenuActions(char* mapName,int* playIsPush, int* menuOpen,int* aideOpen, GLuint casechecked){
	SDL_Event e;
	bool askedForQuit = false;
	while(SDL_PollEvent(&e) && !askedForQuit) {
		switch(e.type) {
		 	case SDL_MOUSEBUTTONDOWN:
         			if (e.button.button==SDL_BUTTON_LEFT){
					float x = 800.0*(e.button.x/(float)WINDOW_WIDTH)-400.0;
					float y = 600.0*(e.button.y/(float)WINDOW_HEIGHT)-300.0;
				clicButton (e, playIsPush, x ,y, menuOpen,aideOpen, mapName, casechecked);
					
				}
			break;
			case SDL_QUIT : askedForQuit = true;
			break;
			case SDL_KEYDOWN :
				switch(e.key.keysym.sym){
					case 'q' : 
					case SDLK_ESCAPE : 
					askedForQuit = true;
					break;
					default : break;
				}
			break;
			default : break;
		}
	}
	return askedForQuit;
}

//Variables globales pour la gestion de la caméra
bool isMouseExtremRight = false;
bool isMouseExtremLeft = false;
bool isMouseExtremTop = false;
bool isMouseExtremBottom = false;
//Gestion de la fin du jeu
Uint32 startEndGameTime = 0;
bool handleGameActions(World* world, Interface* interface){
	//Gestion de l'évolution de l'argent
	bool gameIsFinished = false;
	
	//Le jeu est-il fini ?
	if(world->gameLoosed || world->gameWinned){
		if(startEndGameTime == 0) startEndGameTime = SDL_GetTicks();
		Uint32 elapsedTime = SDL_GetTicks() - startEndGameTime;
		if (elapsedTime > END_GAME_DURATION) gameIsFinished = true;
		else{
			gameIsFinished = false;
		}
		//On ne traite pas les actions de l'utilisateur en fin de jeu
		return gameIsFinished;
	}
	
	SDL_Event e;
	bool askedForQuit = false;
	while(SDL_PollEvent(&e) && !askedForQuit) {
		/* L'utilisateur ferme la fenêtre : */
		if(e.type == SDL_QUIT) {
			askedForQuit = true;
		}
		
		else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
			askedForQuit = handleGameKeyboard(&(e.key), world, interface);
		}	
		
		else askedForQuit = handleGameMouse(&e, world, interface);
	}
	//Gestion caméra
	
	if(isMouseExtremBottom) world->cameraPosition.y += 2;
	else if(isMouseExtremTop) world->cameraPosition.y -= 2;
	if(isMouseExtremLeft) world->cameraPosition.x += 2;
	else if(isMouseExtremRight) world->cameraPosition.x -= 2;

	return askedForQuit;
}

bool handleGameKeyboard(const SDL_KeyboardEvent* e, World* world, Interface* interface){
	bool askedForQuit = false;
	
	switch((*e).keysym.sym){
		case 'q' : 
		case SDLK_ESCAPE : 
		askedForQuit = true;
		break;
		case ' ' : 
			if((*e).type != SDL_KEYDOWN) break;//Sinon met pause et l'enlève dès qu'on retire espace
			if(world->paused){
				world->paused = false;
				interface->currentAction = NO_ACTION;
			}
			else{
				world->paused = true;
				interface->currentAction = PAUSE_GAME;
			}
		break;
		default : break;
	}
	
	return askedForQuit;
}

//TODO
bool handleGameMouse(const SDL_Event* e, World* world, Interface* interface){
	if(e->type != SDL_MOUSEMOTION && e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEBUTTONUP)
		return false;
	if(world == NULL || interface == NULL) return false;
	//Déplacement de la caméra si on est à une extrémité de la fenêtre
	if(e->type == SDL_MOUSEMOTION && SDL_GetTicks() > 2000){
		if(fabs((int)e->motion.x - (int)WINDOW_WIDTH) < 2.* WINDOW_WIDTH / 100.0){
			isMouseExtremRight = true;
			isMouseExtremLeft = false;
		}
		else if(fabs(e->motion.x - 0.0) < 2.*WINDOW_WIDTH / 100.0){
			isMouseExtremLeft = true;
			isMouseExtremRight = false;
		}
		else {
			isMouseExtremLeft = false;
			isMouseExtremRight = false;
		}
		if(fabs((int)e->motion.y - (int)WINDOW_HEIGHT) < 2.*WINDOW_HEIGHT / 100.0){
			isMouseExtremBottom = true;
			isMouseExtremTop = false;
		}
		else if(fabs(e->motion.y - 0.) < 2.* WINDOW_HEIGHT / 100.0){
			isMouseExtremTop = true;
			isMouseExtremBottom = false;
		}	
		else{
			isMouseExtremBottom = false;
			isMouseExtremTop = false;
		}
	}
	else if(e->type == SDL_MOUSEBUTTONDOWN){
		Tower* pointedTower = NULL;
		Action action = detectAction(e->button.x, e->button.y, world, interface, &pointedTower);
		if(action == QUIT_GAME) return true;
		if(action == CLICK_ON_TOWER) {
			if(pointedTower == NULL) fprintf(stderr, "Impossible d'obtenir les infos sur la tour sélectionnée.\n");
			else {
				char towerName[20];
				switch(pointedTower->type){
					case LASER : sprintf(towerName, "LASER");
					break;
					case GUN : sprintf(towerName, "GUN");
					break;
					case HYBRID : sprintf(towerName, "HYBRID");
					break;
					case ROCKET : sprintf(towerName, "ROCKET");
					break;
				}
					
				updateInfoTexture(interface, towerName, pointedTower->power, pointedTower->rate, pointedTower->range);
			}
		}
		else if(action == CLICK_ON_MAP){
		
			switch(interface->currentAction){
			case PUT_GUN : 
				addTowerOnMap(world, e->button.x, e->button.y, GUN);
				interface->currentAction = NO_ACTION;
			
			break;
			case PUT_HYBRID :
				addTowerOnMap(world, e->button.x, e->button.y, HYBRID);
				interface->currentAction = NO_ACTION;
			break;
			case PUT_LASER :
				addTowerOnMap(world, e->button.x, e->button.y, LASER);
				interface->currentAction = NO_ACTION;
			break;
			case PUT_ROCKET :
				addTowerOnMap(world, e->button.x, e->button.y, ROCKET);
				interface->currentAction = NO_ACTION;
			break;
			case NO_ACTION : 
			break;
			case QUIT_GAME :
			break;
			case CLICK_ON_MAP:
			break;
			case PAUSE_GAME : 
			default :
			break;
			}
		}
		else{
			//Pour l'instant quand le jeu est en pause on ne peut que quitter (à modifier)
			if(interface->currentAction != PAUSE_GAME){
				interface->currentAction = action;
			}
		}
		
	}
	
	return false;
}

Action detectAction(Uint16 x, Uint16 y, World* world, Interface* interface, Tower** pointedTower){
	bool actionDetected = false;
	
	//Inutile de tester tous les boutons si on n'est pas sur l'interface
	if(isMouseOnInterface(x,y, interface)) {
		goToHeadList(interface->lstButtons);
		Button* cur = NULL;
		while( !actionDetected && (cur = (Button*) nextData(interface->lstButtons)) != NULL){
			actionDetected = isMouseOnButton(cur, x, y);	
		}
		if(cur == NULL) return NO_ACTION;
		return cur->action;
	}
	else{
	//On check le click sur une tour
		Tower* cur = NULL;
		goToHeadList(world->towersList);
		while(!actionDetected && (cur = (Tower*) nextData(world->towersList)) != NULL ){
			actionDetected = isMouseOnTower(cur, world->cameraPosition, x, y);
		}
		if(actionDetected){
			*pointedTower = cur;
			return CLICK_ON_TOWER;
		}
		else return CLICK_ON_MAP;
	}
}

bool isMouseOnInterface(Uint16 x, Uint16 y, Interface* interface){
	if(interface == NULL) return false;
	bool inside = false;
	Point3D oglMouse = sdlToOpenGL(PointXYZ(x,y,0.0));
	inside = oglMouse.x >= (interface->position.x - interface->width / 2.0) && oglMouse.x <= interface->position.x + (interface->width / 2.0);
	if(!inside) return inside;
	inside = oglMouse.y >= interface->position.y - (interface->height / 2.0)  && oglMouse.y <= interface->position.y + interface->height / 2.0;
	return inside;
}

bool isMouseOnButton(Button* button, Uint16 x, Uint16 y){
	if(button == NULL) return false;
	bool inside = false;
	Point3D oglMouse = sdlToOpenGL(PointXYZ(x,y,0.0));
	inside = oglMouse.x >= button->position.x - button->width / 2.0 && oglMouse.x <= button->position.x + button->width / 2.0;
	if(!inside) return inside;
	inside = oglMouse.y >= button->position.y - button->height / 2.0 && oglMouse.y <= button->position.y + button->height;
	return inside;
}

//ATTENTION : la caméra est à prendre en compte dans la détection d'un clic sur une tour
bool isMouseOnTower(Tower* tower, Point3D cameraPosition, Uint16 x, Uint16 y){
	if(tower == NULL) return false;
	bool inside = false;
	Point3D oglMouse = sdlToOpenGL(PointXYZ(x,y,0.0));
	//Point3D oglTower = sdlToOpenGL(tower->position);
	Point3D realTowerPos = tower->position;
	//Il faut appliquer la translation de la caméra à la position
	//"sdl to opengl" de la tour pour avoir sa position dans le monde
	realTowerPos.x += cameraPosition.x;
	realTowerPos.y += cameraPosition.y;
	realTowerPos.z += cameraPosition.z;
	
	inside = oglMouse.x >= realTowerPos.x - TOWER_WIDTH_PX / 2.0 && oglMouse.x <= realTowerPos.x + TOWER_HEIGHT_PX / 2.0;
	if(!inside) return inside;
	inside = oglMouse.y >= realTowerPos.y - TOWER_HEIGHT_PX / 2.0 && oglMouse.y <= realTowerPos.y + TOWER_HEIGHT_PX;
	return inside;
}

void clicButton (SDL_Event e,int* playIsPush, float x, float y, int* menuOpen,int* aideOpen,char* mapName, GLuint casechecked){

			if (x >= 60. && x <= 178. && y >= -180. && y <= -60. && *aideOpen ==0){
				*menuOpen = 1;
			}

			if (x >= 194. && x <= 363. && y >= -135. && y <= -105. && *menuOpen ==1){
				strcpy(mapName, "map1.itd");


			}
			if (x >= 200. && x <= 384. && y >= -95. && y <= -66. && *menuOpen ==1){
				strcpy(mapName, "map2.itd");

			}
			if (x >= -180. && x <= -60. && y >= -181. && y <= -60.){
				*aideOpen =1;
			}
			if (x >= 190. && x <= 212. && y >= -212. && y <= -194. && *aideOpen==1 ){
				*aideOpen =0;
			}
			if (x >= -61. && x <= 58. && y >= 60. && y <= 178. ){
				*playIsPush =1;
			}

				  
	

}
