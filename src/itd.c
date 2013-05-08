#ifdef MAC
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_ttf.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else 
#include <unistd.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_ttf.h>
#endif
#include <stdlib.h>
#include <stdio.h>

#include "itd.h"
#include "graphics.h"
#include "world.h"
#include "list.h"
#include "actionManager.h"
#include "mapDrawer.h"
#include "interfaceDrawer.h"

/* pour avoir le chemin jusqu'à l'executable, pour avoir les chemins des polices*/
 char* rootPath = NULL;

/**
 * Afficher menu
 * Initialiser la map demandée
 * Lancer la boucle de jeu
 */

int main(int argc,  char* argv[]) {
	/*Initialisation SDL, OpenGL etc */
	if( initWindow() == EXIT_FAILURE){
		perror("Impossible d'initialiser la fenêtre SDL, le programme va fermer.\n");
		exit(-1);
	}

	/* initialisation de SDL_TTF*/
	if(TTF_Init()== -1){
		printf("Error loading TTF: %s\n",TTF_GetError());
		exit(1);
	}
	/* initialisation du rootpath */
	int taille = strlen(argv[0]);
	argv[0][taille-7] = 0;
	rootPath = argv[0];

	//Surtout à appeler APRES avoir initialisé la SDL
	World world = initWorld("map/map1.itd");

/*-------------- GESTION DU MENU --------------------*/
	//TODO
	bool mapChosen = false;//Pour debug, à remettre à false pour de vrai
	bool askedForQuit1=false;
	char mapName[30]= "Not chosen";
	TTF_Font* police = NULL;
	SDL_Surface* text=drawMapMenu(police); /* première carte */

	GLuint helpButton = makeTextureFromFile("images/monstrehelp.png");
	GLuint MapMenu = makeTextureFromSurface (text);
	GLuint mapButton = makeTextureFromFile("images/monstrecarte.png");


	
	while(mapChosen == false || askedForQuit1 == false) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();

		/* Placer ici le code de dessin du menu */
		drawMenu(helpButton,MapMenu,mapButton);

		TTF_CloseFont(police);

		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();

		/* Renvoie une chaine de caractère contenant le nom
		du fichier ITD choisi par l'utilisateur ou NULL si rien n'a encore été choisi */
		handleMenuActions(mapName);
		if(strcmp(mapName,"Not chosen") != 0) mapChosen = true;
		
		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}

		askedForQuit1 = handleGameActions();

	}
	

/*-------------- GESTION DU JEU --------------------*/
	//TODO
	bool gameFinished = false, askedForQuit2 = false;
	initGameGraphics(world.map.image);
	while(!gameFinished && !askedForQuit2) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* On tente un nouveau cycle de tours de jeu si besoin. Le temps est 
		 géré par la fonction. La plupart du temps plusieurs tours de jeu sont
		 joués d'affilé. */
		gameFinished = worldNewStep(&world);
		 
		/* Calcul du temps écoulé, si temps < 10 ms, on ne passe pas 
		au tour suivant.
		 */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on ne dessine rien. */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			 drawWorld(&world);
			 drawInterface();
			 /* Echange du front et du back buffer : mise à jour de la fenêtre */
			SDL_GL_SwapBuffers();
      			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    			
		}
		
		/* Boucle traitant les evenements */
		askedForQuit2 = handleGameActions();

	}

	/* Liberation des ressources associées à la SDL */ 
	TTF_Quit;	
	SDL_Quit();

	return EXIT_SUCCESS;
}


