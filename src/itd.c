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
}
