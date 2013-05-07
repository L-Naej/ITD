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
		
		else askedForQuit = handleGameMouse(&e, world);
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
bool handleGameMouse(const SDL_Event* e, World* world){
	if(e->type != SDL_MOUSEMOTION && e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEBUTTONUP)
		return false;
	
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
	
	return false;
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
