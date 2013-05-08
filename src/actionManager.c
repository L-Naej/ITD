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
#include "itd.h"
#include "graphics.h"
#include "interfaceDrawer.h"

void handleMenuActions(char* mapName,int* playIsPush, int* menuOpen, GLuint casechecked){
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		 	case SDL_MOUSEBUTTONDOWN:
         			if (e.button.button==SDL_BUTTON_LEFT){
					float x = 800.0*(e.button.x/(float)WINDOW_WIDTH)-400.0;
					float y = 600.0*(e.button.y/(float)WINDOW_HEIGHT)-300.0;
		printf(" x : %f, y : %f\n",x, y);
				clicButton (e, playIsPush, x ,y, menuOpen, mapName, casechecked);
					
				}
			break;
			default : break;
		}
	}

}

bool handleGameActions(){
	SDL_Event e;
	bool askedForQuit = false;
	while(SDL_PollEvent(&e)) {
		/* L'utilisateur ferme la fenêtre : */
		if(e.type == SDL_QUIT) {
			askedForQuit = true;
			break;
		}
		switch(e.type) {
		case SDL_KEYDOWN:
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

void clicButton (SDL_Event e,int* playIsPush, float x, float y, int* menuOpen,char* mapName, GLuint casechecked){

			if (x >= 103. && x <= 195. && y >= -137. && y <= -46.){
				*menuOpen = 1;
			}

			if (x >= 225. && x <= 314. && y >= -135. && y <= -107. && *menuOpen ==1){
				strcpy(mapName, "map1");


			}
			if (x >= 220. && x <= 340. && y >= -97. && y <= -66. && *menuOpen ==1){
				strcpy(mapName, "map2");

			}

				  
	

}
