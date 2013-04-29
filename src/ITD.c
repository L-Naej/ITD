#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Map.h"
#include "sdl_tools.h"
#include "tower.h"
#include "interfaceDrawer.h"
#include "ActionManager.h"
#include "ITD.h"


SDL_Surface* ecran= NULL;

int initWindow(){
	/* Initialisation de la SDL */

	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	 ecran = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
	 if(ecran == NULL ) {
    		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    		exit(EXIT_FAILURE);
  	}

	/* Titre de la fenêtre */
	SDL_WM_SetCaption("ITD Avatanéo Camarasa Chiganne", NULL);
	return 1;

}

/* ________________________________ MAIN ____________________________________*/

int main(int argc, char** argv) {
	Map map = initMap();
	Tower* rocket = (Tower*) malloc (sizeof(Tower));
	Tower* laser = (Tower*) malloc (sizeof(Tower));
	Tower* mitraillette = (Tower*) malloc (sizeof(Tower));
	Tower* hybrid = (Tower*) malloc (sizeof(Tower));
	loadMap(&map,rocket, laser, mitraillette, hybrid);
	/*dumpMap(map);*/

	/* récupération de l'image du boutton HELP, pour le moment en ligne de commande  */
	char* nom =argv[1];
	
	chdir("images/");


	initWindow();


	GLuint texture = makeTexture (nom);
	/* Boucle d'affichage du menu */

	int loop = 1;
	while(loop) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();

		/* dessin du menu */
   		glClear(GL_COLOR_BUFFER_BIT);
    		glMatrixMode(GL_MODELVIEW); 
    		glLoadIdentity();

		drawMapMenu();

		/*glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

		glPushMatrix();
		glColor3ub(0,255,0);
		glTranslatef(-0.25, 0.5, 0);
		drawCarre();
		glPopMatrix();*/


		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();

		/* Boucle traitant les evenements */
		/*handleActions();*/
		SDL_Event e;
    		while(SDL_PollEvent(&e)) {
      			if(e.type == SDL_QUIT) {
       				 loop = 0;
       			 break;
			}
     		 

		switch (e.type){
			case SDL_MOUSEBUTTONUP:
				clicButton(e);
			break;
			case SDL_KEYDOWN:
		  		switch(e.key.keysym.sym){
				    	case 'q' : 
						loop = 0;
					break;
		    			case SDLK_ESCAPE : 
						loop = 0;
					break;
					
					break;
					  default : break;
				}
		  	break;
		}
		
	}
		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/*Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}
	}

	

		/* Liberation des ressources associées à la SDL */ 
		SDL_Quit();

		return EXIT_SUCCESS;
}


