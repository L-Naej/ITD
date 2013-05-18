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
	interface.lstButtons = NULL;
	char* mapName= NULL;
	BUTTON_OF_MENU.lstMapName = NULL;
	BUTTON_OF_MENU.lstMapButton = NULL;
	BUTTON_OF_MENU.lstMapTextureIndex = NULL;
	bool play = false;
	
	List* lstMaps = createEmptyList();
	readDirectory(lstMaps);
	/* selection d'une carte en ligne de commande*/
	if (argc >= 2 && argv[1]){
		char* curMap = NULL;
		while( (curMap = (char*) nextData(lstMaps)) != NULL){
			if (strcmp(argv[1],curMap)==0){
				mapName = (char*) malloc(strlen(argv[1])*sizeof(char));
				if(mapName == NULL){
					fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire.\n");
					exit(EXIT_FAILURE);
				}
				strcpy(mapName,argv[1]);
				play = true;
				break;	
			}	

		}
		if(! play) fprintf(stderr, "Erreur le nom donné en paramètre ne correspond à aucun fichier map\n");
		
	}
	freeListComplete(lstMaps);
	
/*-------------- GESTION DU MENU --------------------*/
do{

	interface.lstButtons = NULL;

	/* chargement des polices */
	int playIsPush = 0;
	int menuOpen = 0;
	int aideOpen = 0;
	initMenuGraphics();


	/* ouverture du répertoire data */

	while(!play && askedForQuit == false) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();

		/* Placer ici le code de dessin du menu */		
		drawMenu(&menuOpen,&aideOpen,&playIsPush,mapName);

		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();

		/* Renvoie une chaine de caractère contenant le nom
		du fichier ITD choisi par l'utilisateur ou NULL si rien n'a encore été choisi */
		askedForQuit = handleMenuActions(&mapName,&playIsPush, &menuOpen,&aideOpen);

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
	
	
	float width = .15;//10% de largeur
	float height = 1.; //Toute la hauteur
	float positionX = 0.85; //A 90% de la largeur
	float positionY = .0; //A 100% de la hauter
	
	if(!askedForQuit){
		strcat(mapPath, mapName);
		world = initWorld(mapPath);
		initGameGraphics();
		GAME_TEXTURES_ID.MAP_ID = makeTextureFromSurface(world.map.image);

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
	play = false;
	mapName = NULL;
	
	cleanWorld(&world);
	cleanInterface(&interface);
	
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


