#ifdef MAC
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else 
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
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
	
	//Surtout à appeler APRES avoir initialisé la SDL
	World world = initWorld("map/map1.itd");
/*-------------- GESTION DU MENU --------------------*/
	//TODO
	bool mapChosen = true;//Pour debug, à remettre à false pour de vrai
	char mapName[MAX_FILE_LENGTH];
	
	while(mapChosen == false) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();

		/* Placer ici le code de dessin du menu */
		drawMenu();
		
		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();

		/* Renvoie une chaine de caractère contenant le nom
		du fichier ITD choisi par l'utilisateur ou NULL si rien n'a encore été choisi */
		handleMenuActions(mapName);
		if(mapName != NULL) mapChosen = true;
		
		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}
	}
	

/*-------------- GESTION DU JEU --------------------*/
	//TODO
	bool gameFinished = false, askedForQuit = false;
	initGameGraphics(world.map.image);
	
	//Initialisation interface
	float width = .10;//10% de largeur
	float height = 1.; //Toute la hauteur
	float positionX = 0.90; //A 90% de la largeur
	float positionY = .0; //A 100% de la hauter
	Interface interface = initGameInterface(width, height, positionX, positionY);
	while(!gameFinished && !askedForQuit) {
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
			 drawInterface(&interface);
			 /* Echange du front et du back buffer : mise à jour de la fenêtre */
			SDL_GL_SwapBuffers();
      			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    			
		}
		
		/* Boucle traitant les evenements */
		askedForQuit = handleGameActions(&world, &interface);
	}

	/* Liberation des ressources associées à la SDL */ 
	SDL_Quit();

	return EXIT_SUCCESS;
}


