#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "ITD.h"


extern SDL_Surface* ecran;



void drawButton(){
	glBegin(GL_QUADS);
 	glTexCoord2f(1, 0);
    	glVertex2f(-0.15,-0.15);
 	glTexCoord2f(1, 1);
    	glVertex2f(-0.15,0.15);
 	glTexCoord2f(0, 1);
    	glVertex2f(0.15,0.15);
 	glTexCoord2f(0, 0);
    	glVertex2f(0.15,-0.15);
    glEnd();
}

void drawCarre(){
	glBegin(GL_QUADS);
    	glVertex2f(-0.15,-0.15);
    	glVertex2f(-0.15,0.);
    	glVertex2f(0.,0.);
    	glVertex2f(0.,-0.15);
    glEnd();
}


void drawMapMenu(){

	if(TTF_Init()== -1){
		printf("Error loading TTF: %s\n",TTF_GetError());
		exit(1);
	}


	TTF_Font* police = NULL;
	police = TTF_OpenFont("/home/camarasa/Documents/ITD/font/lighthouse.ttf",0.5);




	SDL_Color blanc = {255, 255, 255};

	SDL_Surface* text = NULL;



	text = TTF_RenderText_Solid(police, "Coucou! Salut, blabla",blanc);
	if (text == NULL){
		printf("error");
	}


		SDL_Rect position;
		position.x = 0;
		position.y = 0;
		SDL_BlitSurface(text, NULL, ecran ,&position);




	TTF_CloseFont(police);
	TTF_Quit;
	
}

GLuint makeTexture (char* path){
	GLuint texture;
	GLenum format;
	/*char* menuHelp = "images/monstrehelp.png";*/
	SDL_Surface* image = IMG_Load(path);
	if (image ==NULL){
		printf("error chargement\n");
		exit(1);
	}
	
	printf("image chargée\n");

	/* création de la texture */
	glGenTextures(1,&texture);/* allocation de la mémoire*/
	glBindTexture(GL_TEXTURE_2D,texture); /* bindage*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);


	switch(image->format->BytesPerPixel) {
		case 1:
		format = GL_RED;
		break;

		case 3:
		format = GL_RGB;
		break;

		case 4:
		format = GL_RGBA;
		break;

		default:
		fprintf(stderr, "Format des pixels de l’image  non pris en charge\n");
		return EXIT_FAILURE;
	}

	/*envoi de la texture à openGL*/
	glTexImage2D(GL_TEXTURE_2D,0,4, image->w, image->h, 0, format,GL_UNSIGNED_BYTE, image->pixels); 
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image);
	
	return texture;
}
