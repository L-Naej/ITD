#ifdef MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else 
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_ttf.h>
#endif
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "itd.h"
#include "graphics.h"
#include "world.h"
#include "list.h"
#include "actionManager.h"
#include "mapDrawer.h"
#include "interfaceDrawer.h"

#define MAX_LENGHT 30

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
	bool askedForQuit = false;
	World world;
	world.towersList = NULL;
	world.map.name = NULL;
	world.map.tabXYConstruct = NULL;
	world.map.image = NULL;
	world.map.pathNodeList = NULL;
	Interface interface;
	
/*-------------- GESTION DU MENU --------------------*/
do{
	bool play = false;
	char mapName[30]= "Not chosen";


	/* chargement des polices */
	int playIsPush = 0;
	int menuOpen = 0;
	int aideOpen = 0;

	/* ouverture du répertoire data */

	initMenuGraphics();
	while(play == false && askedForQuit == false) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();

		/* Placer ici le code de dessin du menu */		
		drawMenu(MENU_TEXTURES_ID.MAPS,MENU_TEXTURES_ID.nb_cartes,&menuOpen,&aideOpen,&playIsPush,mapName);



		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();

		/* Renvoie une chaine de caractère contenant le nom
		du fichier ITD choisi par l'utilisateur ou NULL si rien n'a encore été choisi */
		askedForQuit = handleMenuActions(mapName,&playIsPush, &menuOpen,&aideOpen);

		if(playIsPush == 2) play = true;
		
		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}

	}
	

/*-------------- GESTION DU JEU --------------------*/
	bool gameFinished = false;
	//Surtout à appeler APRES avoir initialisé la SDL
	char mapPath[50] = "data/";
	strcat(mapPath, mapName);
	
	float width = .15;//10% de largeur
	float height = 1.; //Toute la hauteur
	float positionX = 0.85; //A 90% de la largeur
	float positionY = .0; //A 100% de la hauter
	
	if(!askedForQuit){
		world = initWorld(mapPath);
		initGameGraphics(world.map.image);

		//Initialisation interface
		interface = initGameInterface(width, height, positionX, positionY);
	
		startWorld(&world);
	}
	while(!gameFinished && !askedForQuit) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* On tente un nouveau cycle de tours de jeu si besoin. Le temps est 
		 géré par la fonction. La plupart du temps plusieurs tours de jeu sont
		 joués d'affilé. */
		worldNewStep(&world);
		 
		drawWorld(&world);
		drawInterface(&interface, &world);
		/* Calcul du temps écoulé, si temps < 10 ms, on ne passe pas 
		au tour suivant.
		 */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on ne dessine rien. */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {

			 /* Echange du front et du back buffer : mise à jour de la fenêtre */
			SDL_GL_SwapBuffers();
      			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    			
		}
		
		/* Boucle traitant les evenements */
		askedForQuit = handleGameActions(&world, &interface, &gameFinished);
	}
	cleanWorld(&world);
}while(! askedForQuit);
	/* Liberation des ressources */ 
	cleanExit(&world, &interface);

	return EXIT_SUCCESS;
}

void cleanExit(World* world, Interface* interface){
	cleanWorld(world);
	cleanInterface(interface);
	free(BUTTON_OF_MENU.regles);
	free(BUTTON_OF_MENU.choix_carte);
	free(BUTTON_OF_MENU.jouer);
	free(BUTTON_OF_MENU.close_rules);	
	TTF_Quit();	
	SDL_Quit();
}


