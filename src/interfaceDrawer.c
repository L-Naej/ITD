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




extern char* rootPath;


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
	
	char* font = (char*)malloc(sizeof(char)*(strlen(rootPath)+19));
	strcpy(font,rootPath);
	font = strcat(font,"font/lighthouse.ttf");

	police = TTF_OpenFont(font,1);

	SDL_Color blanc = {255, 255, 255};

	SDL_Surface* text = NULL;



	text = TTF_RenderText_Solid(police, "Coucou! Salut, blabla",blanc);
	if (text == NULL){
		printf("error");
	}


	return text;
	
	
}


