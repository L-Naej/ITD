#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include "Map.h"
#include "sdl_tools.h"
#include "tower.h"
#include "interfaceDrawer.h"
#include "ActionManager.h"
#include "ITD.h"
#include "utils.h"


extern char* rootPath;


int initWindow(){
	/* Initialisation de la SDL */

	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	 if(SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE) == NULL ) {
    		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    		exit(EXIT_FAILURE);
  	}

	/* Titre de la fenêtre */
	SDL_WM_SetCaption("ITD Avatanéo Camarasa Chiganne", NULL);
	return 1;

}

/* ________________________________ MAIN ____________________________________*/

int main(int argc, char** argv) {

	/* initialisation de SDL_TTF*/
	if(TTF_Init()== -1){
		printf("Error loading TTF: %s\n",TTF_GetError());
		exit(1);
	}
	int taille = strlen(argv[0]);
	argv[0][taille-7] = 0;
	rootPath = argv[0];



	Map map = initMap();
	Tower* rocket = (Tower*) malloc (sizeof(Tower));
	Tower* laser = (Tower*) malloc (sizeof(Tower));
	Tower* mitraillette = (Tower*) malloc (sizeof(Tower));
	Tower* hybrid = (Tower*) malloc (sizeof(Tower));
	loadMap(&map,rocket, laser, mitraillette, hybrid);

	/* recupération du chemin de l'image "help" */
	char* helpButtonPath = (char*)malloc(sizeof(char)*(strlen(rootPath)+22));
	strcpy(helpButtonPath,rootPath);
	helpButtonPath = strcat(helpButtonPath,"images/monstrehelp.png");

	/* recupération du chemin de l'image "carte" */
	char* mapButtonPath = (char*)malloc(sizeof(char)*(strlen(rootPath)+23));
	strcpy(mapButtonPath,rootPath);
	mapButtonPath = strcat(mapButtonPath,"images/monstrecarte.png");



	TTF_Font* police = NULL;

	/* création des surface des boutons */
	SDL_Surface* helpTexture = IMG_Load(helpButtonPath);

	SDL_Surface* mapTexture = IMG_Load(mapButtonPath);


	SDL_Surface* text=drawMapMenu(police); /* première carte */


	initWindow();


	GLuint helpButton = makeTexture (helpTexture);
	GLuint mapButton = makeTexture (mapTexture);
	GLuint MapMenu = makeTexture (text);
	/* Boucle d'affichage du menu */
		
		


	int loop = 1;
	while(loop) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();

		/* dessin du menu */
   		glClear(GL_COLOR_BUFFER_BIT);
    		glMatrixMode(GL_MODELVIEW); 
    		glLoadIdentity();


/* _________________ Dessin du sous-menu pour choisir la carte( qui marche pas )_______________*/
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, MapMenu);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		


	
	TTF_CloseFont(police);

/* _________________ Dessin du bouton d'aide_______________*/

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, helpButton);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

/* _________________ Dessin du bouton pour choisir la carte_______________*/

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mapButton);

		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glTranslatef(0.5,-0.5,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);


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
		TTF_Quit;
		SDL_Quit();

		return EXIT_SUCCESS;
}


