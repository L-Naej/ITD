#include "actionManager.h"
#include <SDL/SDL.h>
#ifdef MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <math.h>
#include "graphics.h"

void handleMenuActions(char* mapName){}

//Variables globales pour la gestion de la caméra
bool isMouseExtremRight = false;
bool isMouseExtremLeft = false;
bool isMouseExtremTop = false;
bool isMouseExtremBottom = false;

bool handleGameActions(World* world, Interface* interface){
	SDL_Event e;
	bool askedForQuit = false;
	while(SDL_PollEvent(&e) && !askedForQuit) {
		/* L'utilisateur ferme la fenêtre : */
		if(e.type == SDL_QUIT) {
			askedForQuit = true;
		}
		
		else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
			askedForQuit = handleGameKeyboard(&(e.key), world);
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

bool handleGameKeyboard(const SDL_KeyboardEvent* e, World* world){
	bool askedForQuit = false;
	
	switch((*e).keysym.sym){
		case 'q' : 
		case SDLK_ESCAPE : 
		askedForQuit = true;
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
		Action action = detectAction(e->button.x, e->button.y, world, interface);
		switch(action){
		case PUT_GUN : printf("gun\n");
		break;
		case PUT_LASER : printf("laser\n");
		break;
		case PUT_ROCKET : printf("rocket\n");
		break;
		case PUT_HYBRID : printf("hybrid\n");
		break;
		case QUIT_GAME : printf("quit\n");
		break;
		case NO_ACTION : printf("no action\n");
		break;
		default: printf("caca\n"); break;
		}
	}
	
	return false;
}

Action detectAction(Uint16 x, Uint16 y, World* world, Interface* interface){
	if(isMouseOnInterface(x,y, interface)) {
		goToHeadList(interface->lstButtons);
		Button* cur = NULL;
		bool actionDetected = false;
		while( !actionDetected && (cur = (Button*) nextData(interface->lstButtons)) != NULL){
			actionDetected = isOnButton(cur, x, y);	
		}
		if(cur == NULL) return NO_ACTION;
		return cur->action;
	}
	else return NO_ACTION;
}

bool isMouseOnInterface(Uint16 x, Uint16 y, Interface* interface){
	if(interface == NULL) return false;
	bool inside = false;
	Point3D oglMouse = sdlToOpenGL(PointXYZ(x,y,0.0));
	inside = oglMouse.x >= interface->position.x && oglMouse.x <= interface->position.x + interface->width;
	if(!inside) return inside;
	inside = oglMouse.y >= interface->position.y - interface->height && oglMouse.y <= interface->position.y;
	return inside;
}

bool isOnButton(Button* button, Uint16 x, Uint16 y){
	if(button == NULL) return false;
	bool inside = false;
	Point3D oglMouse = sdlToOpenGL(PointXYZ(x,y,0.0));
	inside = oglMouse.x >= button->position.x && oglMouse.x <= button->position.x + button->width;
	if(!inside) return inside;
	inside = oglMouse.y >= button->position.y - button->height && oglMouse.y <= button->position.y;
	return inside;
}

void clicButton (SDL_Event e){
	if (e.button.button==SDL_BUTTON_LEFT){
		/*printf(" x : %f, y : %f\n",(8.0*(e.button.x/(float)WINDOW_WIDTH)-4.0),(8.0*(e.button.y/(float)WINDOW_WIDTH)-4.0));*/
			if ((8.0*(e.button.x/(float)WINDOW_WIDTH)-4.0) >= -0.6 && 
			(8.0*(e.button.x/(float)WINDOW_WIDTH)-4.0) <= 0.6 && 
			(8.0*(e.button.y/(float)WINDOW_WIDTH)-4.0) >= -0.6 && 
			(8.0*(e.button.y/(float)WINDOW_WIDTH)-4.0) <= 0.6){
				printf("coucou\n");
			}

				  
	}

}
