#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#ifdef MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "interfaceDrawer.h"
//TODO
void drawInterface(){}
void drawMenu(){}







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
	glVertex2f(-0.25,-0.15);
	glVertex2f(-0.25,0.);
	glVertex2f(0.,0.);
	glVertex2f(0.,-0.15);
    glEnd();
}


SDL_Surface* drawMapMenu(TTF_Font* police){
	
	police = TTF_OpenFont("/home/camarasa/Documents/ITD/font/lighthouse.ttf",1);

	SDL_Color blanc = {255, 255, 255};

	SDL_Surface* text = NULL;



	text = TTF_RenderText_Solid(police, "Coucou! Salut, blabla",blanc);
	if (text == NULL){
		printf("error");
	}


	return text;
	
	
}

GLuint makeTexture (SDL_Surface* image){
	GLuint texture;
	GLenum format;


	if (image ==NULL){
		printf("error chargement\n");
		exit(1);
	}
	

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
