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
#include <dirent.h>
#include <sys/types.h>

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
	/* initialisation du rootpath */
	char* rootPath;
	int taille = strlen(argv[0]);
	argv[0][taille-7] = 0;
	rootPath = argv[0];

/*-------------- GESTION DU MENU --------------------*/
	//TODO
	bool play = false;
	char mapName[30]= "Not chosen";


	/* chargement des polices */
	TTF_Font* police = NULL;
	char* font1 = (char*)malloc(sizeof(char)*(strlen(rootPath)+19));
	strcpy(font1,rootPath);
	font1 = strcat(font1,"font/Champagne.ttf");
	char* font2 = (char*)malloc(sizeof(char)*(strlen(rootPath)+19));
	strcpy(font2,rootPath);
	font2 = strcat(font2,"font/lighthouse.ttf");

	int playIsPush = 0;
	int menuOpen = 0;
	int aideOpen = 0;

	/* ouverture du répertoire data */
	char* dataPath = (char*)malloc(sizeof(char)*(strlen(rootPath)+5));
	strcpy(dataPath,rootPath);
	dataPath = strcat(dataPath,"data");
	DIR* dataRep = opendir(dataPath);
	if (dataRep ==NULL){
		perror("");
		exit(1);
	}

	struct dirent* fichierLu =NULL;
	
	int i=1;
	char* map[4];

	while((fichierLu = readdir(dataRep))!=NULL && i<= 5){
		if (strcmp(fichierLu->d_name,".")!=0 && strcmp(fichierLu->d_name,"..")!=0){

			if (strlen(fichierLu->d_name)>  MAX_LENGHT)
				printf("Désolé, le nom de la map dépasse 30 caractères\n");
			else{
				map[i]=(char*)malloc(sizeof(char)*strlen(fichierLu->d_name));
				strcpy(map[i],fichierLu->d_name);

				i++;
			}
		}
	}
	printf("1 = %s\n",map[1]);
	printf("2 = %s\n",map[2]);
	printf("3 = %s\n",map[3]);
	printf("4 = %s\n",map[4]);
	printf("5 = %s\n",map[5]);
	printf("6 = %s\n",map[6]);
	int nb_cartes = i-1;
	printf("%d\n",nb_cartes);
	if (closedir(dataRep)!=0){
		printf("erreur dans la fermeture du repertoire\n");
	}

	int j;
	GLuint MapChoice[nb_cartes];
	for (j=1;j<=nb_cartes;j++){
		printf(" %s\n",map[j]);
		SDL_Surface* text=loadFont(police,map[j],font1,100);
		MapChoice[j] = makeTextureFromSurface (text);
	}

	char bienvenue[34]="Bienvenue dans Imac Tower Defense";
	SDL_Surface* bienvenue_surface=loadFont(police,bienvenue,font2,100);
	GLuint Bienvenue = makeTextureFromSurface (bienvenue_surface);

	char choix[18]="Choisir une carte";
	SDL_Surface* choix_surface=loadFont(police,choix,font1,100);
	GLuint ChoixLegend = makeTextureFromSurface (choix_surface);

	char aide[16]="Lire les regles";
	SDL_Surface* aide_surface=loadFont(police,aide,font1,100);
	GLuint ReglesLegend = makeTextureFromSurface (aide_surface);

	char playLegend[7]="Play !";
	SDL_Surface* play_surface=loadFont(police,playLegend,font1,100);
	GLuint PlayLegend = makeTextureFromSurface (play_surface);

	GLuint helpButton = makeTextureFromFile("images/monstrehelp.png");
	GLuint mapButton = makeTextureFromFile("images/monstrecarte.png");
	GLuint playButton = makeTextureFromFile("images/monstreplay.png");
	GLuint casechecked = makeTextureFromFile("images/casechecked.png");
	GLuint casevide = makeTextureFromFile("images/casevide.png");
	GLuint regles = makeTextureFromFile("images/regles.png");
	GLuint bulle = makeTextureFromFile("images/bulle.png");
	bool askedForQuit = false;
	while(play == false && askedForQuit == false) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();

		/* Placer ici le code de dessin du menu */		
		drawMenu(helpButton,mapButton,playButton,casevide,casechecked,Bienvenue,ChoixLegend,PlayLegend,ReglesLegend,regles,bulle,MapChoice,nb_cartes,&menuOpen,&aideOpen,&playIsPush,mapName);

		TTF_CloseFont(police);

		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();

		/* Renvoie une chaine de caractère contenant le nom
		du fichier ITD choisi par l'utilisateur ou NULL si rien n'a encore été choisi */
		askedForQuit = handleMenuActions(mapName,&playIsPush, &menuOpen,&aideOpen, casechecked);

		if(playIsPush == 2) play = true;
		
		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}

	}
	

/*-------------- GESTION DU JEU --------------------*/
	//TODO
	bool gameFinished = false;
	
	//Surtout à appeler APRES avoir initialisé la SDL
	char mapPath[50] = "map/";
	strcat(mapPath, mapName);
	World world = initWorld(mapPath);
	initGameGraphics(world.map.image);
	
	float width = .15;//10% de largeur
	float height = 1.; //Toute la hauteur
	float positionX = 0.85; //A 90% de la largeur
	float positionY = .0; //A 100% de la hauter
	//Initialisation interface
	Interface interface = initGameInterface(width, height, positionX, positionY);
	
	startWorld(&world);
	while(!gameFinished && !askedForQuit) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* On tente un nouveau cycle de tours de jeu si besoin. Le temps est 
		 géré par la fonction. La plupart du temps plusieurs tours de jeu sont
		 joués d'affilé. */
		gameFinished = worldNewStep(&world);
		 
		drawWorld(&world);
		drawInterface(&interface);
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
		askedForQuit = handleGameActions(&world, &interface);
	}

	/* Liberation des ressources associées à la SDL */ 
	TTF_Quit();	
	SDL_Quit();

	return EXIT_SUCCESS;
}


