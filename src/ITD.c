<<<<<<< HEAD
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
	
/*-------------- GESTION DU MENU --------------------*/
	//TODO
	bool mapChosen = false;
	List mapList;
	char mapName[MAX_FILE_LENGTH];
	mapList = initMenu();
	
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
	while(!gameFinished && !askedForQuit) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/**
		 * Placer ici le code qui fait avancer le
		 * monde d'un pas de temps.
		 */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on ne dessine rien. */
		if(elapsedTime >= TIMESTEP_MILLISECONDS) {
			gameFinished = worldNewStep(NULL);
		}
		 
		/* Calcul du temps écoulé, si temps < 10 ms, on ne passe pas 
		au tour suivant.
		 */
		elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on ne dessine rien. */
		if(elapsedTime >= FRAMERATE_MILLISECONDS) {
			 drawWorld();
			 drawInterface();
			 /* Echange du front et du back buffer : mise à jour de la fenêtre */
			SDL_GL_SwapBuffers();
		}
		
		/* Boucle traitant les evenements */
		askedForQuit = handleGameActions();
	}

	/* Liberation des ressources associées à la SDL */ 
	SDL_Quit();

	return EXIT_SUCCESS;
}


int initWindow(){
	/* Initialisation de la SDL */
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
	fprintf(stderr, "Impossible d'initialiser la SDL.\n");
	return EXIT_FAILURE;
	}

	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	if(EXIT_FAILURE == setVideoMode()) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre.\n");
		return EXIT_FAILURE;
	}

	/* Titre de la fenêtre */
	SDL_WM_SetCaption("ITD Avatanéo Camarasa Chiganne", NULL);
	
	return 0;
}

List initMenu(){
	//TODO
	List menuList;
	return menuList;
}

void reshape() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1., 1., -1., 1.);
}

int setVideoMode() {
  if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
    return EXIT_FAILURE;
  }
  
  reshape();
  
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapBuffers();
  
  return 0;
=======
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "monster.h"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


int main(int argc, char** argv) {

  //Chargement de l'image
  char* filename = "img/map.ppm";
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(WINDOW_WIDTH,WINDOW_HEIGHT,32,SDL_OPENGL);
  SDL_Surface* image = IMG_Load(filename);
  if(image == NULL) {
  	fprintf(stderr, "impossible de charger\n");
        return EXIT_FAILURE;
  }
  printf("Image chargée\n");

  int loop = 1;
  while(loop) {
    Uint32 startTime = SDL_GetTicks();

    /* dessin */
    glClear(GL_COLOR_BUFFER_BIT);
   
   /*
   Monster monster; 
   monster = createMonster();
   printf("Monsters: %d \n",monster.money);*/

  
    SDL_GL_SwapBuffers();
    /* ****** */    
    
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
      
      switch(e.type) {       
	case SDL_KEYDOWN:
	  switch(e.key.keysym.sym){
	    case 'q' : 
	    case SDLK_ESCAPE : 
		loop = 0;
		break;
	    default : break;
	  }
          break;
          
        default:
          break;
      }
    }
    
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }
  
  SDL_Quit();
  
  return EXIT_SUCCESS;
>>>>>>> marina
}
