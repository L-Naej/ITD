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

bool handleMenuActions(char** mapName,int* playIsPush, int* menuOpen,int* aideOpen){
	SDL_Event e;
	bool askedForQuit = false;
	while(SDL_PollEvent(&e) && !askedForQuit) {
		switch(e.type) {
		 	case SDL_MOUSEBUTTONDOWN:
         			if (e.button.button==SDL_BUTTON_LEFT){
					clicButton (e, playIsPush, e.button.x ,e.button.y, menuOpen,aideOpen, mapName);
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
					case SDLK_DOWN : scrollMenu(0);
						goToPosition(BUTTON_OF_MENU.lstMapName, BUTTON_OF_MENU.indexButtonClicked);	
						*mapName = (char*) currentData(BUTTON_OF_MENU.lstMapName);
					break;
					case SDLK_UP : scrollMenu(1);
						goToPosition(BUTTON_OF_MENU.lstMapName, BUTTON_OF_MENU.indexButtonClicked);	
						*mapName = (char*) currentData(BUTTON_OF_MENU.lstMapName);
					break;
					case SDLK_RETURN : if(*menuOpen == 1) *menuOpen = 0;
					break;
					default : break;
				}
			break;
			case SDL_VIDEORESIZE :
				setVideoMode(e.resize.w,e.resize.h);
				initMenuGraphics();
			default : break;
		}
	}
	return askedForQuit;
}

//0 = vers le bas, 1 = vers le haut
void scrollMenu(int direction){
	int oldPosition = BUTTON_OF_MENU.indexFirstButtonDisplayed;
	int i = oldPosition;
	int newPosition;
	
	//Sécurité
	if(direction == 0 && (oldPosition + NB_MAP_DISPLAYED) > BUTTON_OF_MENU.lstMapButton->size) return;
	if(direction == 1 && (oldPosition - NB_MAP_DISPLAYED) < 1) return;
	
	Button* curButton = NULL;
	if(oldPosition == 1) goToHeadList(BUTTON_OF_MENU.lstMapButton);
	else goToPosition(BUTTON_OF_MENU.lstMapButton, oldPosition-1);
	while( i < (oldPosition + NB_MAP_DISPLAYED) && (curButton = (Button*) nextData(BUTTON_OF_MENU.lstMapButton)) != NULL){
		curButton->position.z = -2.0;
		i++;
	}
	if(direction == 1){
		newPosition = oldPosition - NB_MAP_DISPLAYED;
		goToPosition(BUTTON_OF_MENU.lstMapButton, newPosition);
	}
	else if(direction == 0){
		newPosition = BUTTON_OF_MENU.lstMapButton->position + 1;
	}
	
	i = newPosition;
	BUTTON_OF_MENU.indexFirstButtonDisplayed = newPosition;
	if(newPosition == 1) goToHeadList(BUTTON_OF_MENU.lstMapButton);
	while( i < (newPosition + NB_MAP_DISPLAYED) && (curButton = (Button*) nextData(BUTTON_OF_MENU.lstMapButton)) != NULL){
		curButton->position.z = 0.0;
		i++;
	}
	BUTTON_OF_MENU.indexButtonClicked = BUTTON_OF_MENU.indexFirstButtonDisplayed;
}

//Variables globales pour la gestion de la caméra
bool isMouseExtremRight = false;
bool isMouseExtremLeft = false;
bool isMouseExtremTop = false;
bool isMouseExtremBottom = false;
//Gestion de la fin du jeu
Uint32 startEndGameTime = 0;
bool mouseInWindow;
bool handleGameActions(World* world, Interface* interface, bool* gameIsFinished){
	//Le jeu est-il fini ?
	if(world->gameLoosed || world->gameWinned){
		if(startEndGameTime == 0) startEndGameTime = SDL_GetTicks();
		Uint32 elapsedTime = SDL_GetTicks() - startEndGameTime;
		if (elapsedTime > END_GAME_DURATION){ 
			*gameIsFinished = true;
			startEndGameTime = 0;
		}
		else{
			*gameIsFinished = false;
		}
		//On ne traite pas les actions de l'utilisateur en fin de jeu
		return false;
	}
	
	SDL_Event e;
	bool askedForQuit = false;
	while(SDL_PollEvent(&e) && !askedForQuit) {
		/* L'utilisateur ferme la fenêtre : */
		if(e.type == SDL_QUIT) {
			askedForQuit = true;
		}
		//Si on perd le focus sur la fenêtre la souris ne doit pas impacter le jeu (caméra...)
		else if(e.type == SDL_ACTIVEEVENT){
			if(e.active.gain == 0) mouseInWindow = false;
			else mouseInWindow = true;
			
		}
		else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
			askedForQuit = handleGameKeyboard(&(e.key), world, interface);
		}	
		
		else if (e.type == SDL_VIDEORESIZE){
			setVideoMode(e.resize.w, e.resize.h);
			initGameGraphics();
			glDeleteTextures(1, &GAME_TEXTURES_ID.MAP_ID);
			GAME_TEXTURES_ID.MAP_ID = makeTextureFromSurface(world->map.image);
			*interface = initGameInterface(interface->relativeWidth, interface->relativeHeight, interface->relativePosX, interface->relativePosY);
			world->cameraPosition.x = 0.0; world->cameraPosition.y = 0.0;
		}
		else askedForQuit = handleGameMouse(&e, world, interface);
	}
	//Gestion caméra
	if(mouseInWindow){
		float distanceBeforeBlock = 20.;
		int x, y;
		SDL_GetMouseState(&x,&y);
		Point3D oglPoint = sdlToOpenGL(PointXYZ(x, y,0.));
		oglPoint.x -= world->cameraPosition.x;
		oglPoint.y -= world->cameraPosition.y;
		Point3D itdPoint = openGLToItd(world->map.width, world->map.height,oglPoint);
		if( itdPoint.x < 0 - distanceBeforeBlock * world->map.width / 100. || itdPoint.x > world->map.width + distanceBeforeBlock * world->map.width / 100.
			|| itdPoint.y < 0 - distanceBeforeBlock* world->map.height / 100. || itdPoint.y > world->map.height + distanceBeforeBlock * world->map.height / 100.){

			isMouseExtremRight = false;
			isMouseExtremLeft = false;
			isMouseExtremTop = false;
			isMouseExtremBottom = false;
		}
		if(isMouseExtremBottom) world->cameraPosition.y += 2;
		else if(isMouseExtremTop) world->cameraPosition.y -= 2;
		if(isMouseExtremLeft) world->cameraPosition.x += 2;
		else if(isMouseExtremRight) world->cameraPosition.x -= 2;
		
	}

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

bool handleGameMouse(const SDL_Event* e, World* world, Interface* interface){
	if(e->type != SDL_MOUSEMOTION && e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEBUTTONUP)
		return false;
	if(world == NULL || interface == NULL) return false;
	float spaceForCapture = 5.;//%age de fenetre capturé comme étant un déplacement caméra
	//Déplacement de la caméra si on est à une extrémité de la fenêtre
	if(e->type == SDL_MOUSEMOTION && SDL_GetTicks() > 2000){
			
		if(fabs((int)e->motion.x - (int)WINDOW_WIDTH) < spaceForCapture* WINDOW_WIDTH / 100.0){
			isMouseExtremRight = true;
			isMouseExtremLeft = false;
		}
		else if(fabs(e->motion.x - 0.0) < spaceForCapture*WINDOW_WIDTH / 100.0){
			isMouseExtremLeft = true;
			isMouseExtremRight = false;
		}
		else {
			isMouseExtremLeft = false;
			isMouseExtremRight = false;
		}
		if(fabs((int)e->motion.y - (int)WINDOW_HEIGHT) < spaceForCapture*WINDOW_HEIGHT / 100.0){
			isMouseExtremBottom = true;
			isMouseExtremTop = false;
		}
		else if(fabs(e->motion.y - 0.) < spaceForCapture* WINDOW_HEIGHT / 100.0){
			isMouseExtremTop = true;
			isMouseExtremBottom = false;
		}	
		else{
			isMouseExtremBottom = false;
			isMouseExtremTop = false;
		}
		
	}
	else if(e->type == SDL_MOUSEBUTTONDOWN){
		/*
		Point3D mouse = sdlToOpenGL(PointXYZ(e->button.x, e->button.y,0.));
		mouse.x -= world->cameraPosition.x;
		mouse.y -= world->cameraPosition.y;
		mouse = openGLToItd(world->map.width, world->map.height,mouse);
		printf("%.0f %.0f\n", mouse.x, mouse.y);
		*/
		if(e->button.button == SDL_BUTTON_RIGHT){
			suppressTower(world, e->button.x, e->button.y);
			return false;
		}
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
					
				updateInfoTexture(interface, towerName, pointedTower->power, pointedTower->rate, pointedTower->range, pointedTower->cost);
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
				int towerCost;
				switch(action){
				case PUT_GUN : towerCost = world->map.towerdatas->costM;
				updateInfoTexture(interface, "GUN", world->map.towerdatas->powerM,world->map.towerdatas->rateM, world->map.towerdatas->rangeM, towerCost);
				break;
				case PUT_HYBRID : towerCost = world->map.towerdatas->costH;
				updateInfoTexture(interface, "HYBRID", world->map.towerdatas->powerH,world->map.towerdatas->rateH, world->map.towerdatas->rangeH, towerCost);
				break;
				case PUT_LASER : towerCost = world->map.towerdatas->costL;
				updateInfoTexture(interface, "LASER", world->map.towerdatas->powerL,world->map.towerdatas->rateL, world->map.towerdatas->rangeL, towerCost);
				break;
				case PUT_ROCKET : towerCost = world->map.towerdatas->costR;
				updateInfoTexture(interface, "ROCKET", world->map.towerdatas->powerR,world->map.towerdatas->rateR, world->map.towerdatas->rangeR, towerCost);
				break;
				default : towerCost = 0;
				}
				if( world->money > towerCost){
					interface->currentAction = action;
				}
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

void suppressTower(World* world, Uint16 x, Uint16 y){
	Tower* cur = NULL;
	goToHeadList(world->towersList);
	bool towerDetected = false;
	while( !towerDetected && (cur = (Tower*) nextData(world->towersList)) != NULL ){
		towerDetected = isMouseOnTower(cur, world->cameraPosition, x, y);
		if(towerDetected){
			int towerCost;
			switch(cur->type){
				case GUN : towerCost = world->map.towerdatas->costM;
				break;
				case HYBRID : towerCost = world->map.towerdatas->costH;
				break;
				case LASER : towerCost = world->map.towerdatas->costL;
				break;
				case ROCKET : towerCost = world->map.towerdatas->costR;
				break;
				default : towerCost = 0;
			}
			world->money += towerCost;
			freeCellByPosition(world->towersList, world->towersList->position);
		}
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
	if(button->position.z < 0.0) return false;
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

void clicButton (SDL_Event e,int* playIsPush, float x, float y, int* menuOpen,int* aideOpen, char** mapName){
			
			if(*menuOpen){
				*aideOpen = 0;
				Button* curButton = NULL;
				if(BUTTON_OF_MENU.indexFirstButtonDisplayed == 1) goToHeadList(BUTTON_OF_MENU.lstMapButton);
				goToPosition(BUTTON_OF_MENU.lstMapButton, BUTTON_OF_MENU.indexFirstButtonDisplayed - 1);
				int i = BUTTON_OF_MENU.indexFirstButtonDisplayed;
				while( i < (BUTTON_OF_MENU.indexFirstButtonDisplayed + NB_MAP_DISPLAYED) && (curButton = (Button*) nextData(BUTTON_OF_MENU.lstMapButton)) != NULL){
					if (isMouseOnButton(curButton,x, y) ==true){
						BUTTON_OF_MENU.indexButtonClicked = BUTTON_OF_MENU.lstMapButton->position;
						goToPosition(BUTTON_OF_MENU.lstMapName, BUTTON_OF_MENU.indexButtonClicked);	
						*mapName = (char*) currentData(BUTTON_OF_MENU.lstMapName);
						break;
					}
					else{ BUTTON_OF_MENU.indexButtonClicked = -1; *mapName = NULL;}
				}
				return;
			}
			
			if (isMouseOnButton(BUTTON_OF_MENU.choix_carte,x, y) ==true ){
				*menuOpen = 1;
				*playIsPush = 0;
				return;
			}
			

			if (isMouseOnButton(BUTTON_OF_MENU.regles,x, y) ==true){
				*aideOpen =1;
				return;
			}
			if (isMouseOnButton(BUTTON_OF_MENU.close_rules,x, y) ==true && *aideOpen==1 ){
				*aideOpen =0;
				return;
			}
			if (isMouseOnButton(BUTTON_OF_MENU.jouer,x, y) ){
				*playIsPush =1;
				return;
			}

}
