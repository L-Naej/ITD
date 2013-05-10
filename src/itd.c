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
	char* rootPath;
	int taille = strlen(argv[0]);
	argv[0][taille-7] = 0;
	rootPath = argv[0];

	//Surtout à appeler APRES avoir initialisé la SDL
	World world = initWorld("map/map1.itd");

/*-------------- GESTION DU MENU --------------------*/
	//TODO
	bool play = false;//Pour debug, à remettre à false pour de vrai
	char mapName[30]= "Not chosen";
	int playIsPush = 0;

	TTF_Font* police = NULL;
	char* font1 = (char*)malloc(sizeof(char)*(strlen(rootPath)+19));
	strcpy(font1,rootPath);
	font1 = strcat(font1,"font/Champagne.ttf");
	char* font2 = (char*)malloc(sizeof(char)*(strlen(rootPath)+19));
	strcpy(font2,rootPath);
	font2 = strcat(font2,"font/lighthouse.ttf");

	int menuOpen = 0;


	char nomcarte1[13]="carte marine";
	SDL_Surface* text1=loadFont(police,nomcarte1,font1,100);
	GLuint MapChoice1 = makeTextureFromSurface (text1);

	char nomcarte2[15]="carte spatiale";
	SDL_Surface* text2=loadFont(police,nomcarte2,font1,100);
	GLuint MapChoice2 = makeTextureFromSurface (text2); 

	char bienvenue[34]="Bienvenue dans Imac Tower Defense";
	SDL_Surface* bienvenue_surface=loadFont(police,bienvenue,font2,100);
	GLuint Bienvenue = makeTextureFromSurface (bienvenue_surface);

	char choix[18]="Choisir une carte";
	SDL_Surface* choix_surface=loadFont(police,choix,font1,100);
	GLuint Choix = makeTextureFromSurface (choix_surface);

	char aide[15]="Besoin d'aide?";
	SDL_Surface* aide_surface=loadFont(police,aide,font1,100);
	GLuint Aide = makeTextureFromSurface (aide_surface);

	char playLegend[7]="Play !";
	SDL_Surface* play_surface=loadFont(police,playLegend,font1,100);
	GLuint Play = makeTextureFromSurface (play_surface);



	GLuint helpButton = makeTextureFromFile("images/monstrehelp.png");
	GLuint mapButton = makeTextureFromFile("images/monstrecarte.png");
	GLuint playButton = makeTextureFromFile("images/monstreplay.png");
	GLuint casechecked = makeTextureFromFile("images/casechecked.png");
	GLuint casevide = makeTextureFromFile("images/casevide.png");




	
	while(play == false) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();

		/* Placer ici le code de dessin du menu */		
		drawMenu(helpButton,mapButton,playButton,casevide,casechecked,Bienvenue,Choix,Aide,Play,MapChoice1,MapChoice2,&menuOpen,mapName);

		TTF_CloseFont(police);

		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();

		/* Renvoie une chaine de caractère contenant le nom
		du fichier ITD choisi par l'utilisateur ou NULL si rien n'a encore été choisi */
		handleMenuActions(mapName,&playIsPush, &menuOpen,casechecked);

		if(playIsPush == 1) play = true;
		
		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}



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


