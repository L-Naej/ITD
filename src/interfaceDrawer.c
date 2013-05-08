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

void drawMenu(GLuint helpButton,GLuint MapMenu,GLuint mapButton){
		glClear(GL_COLOR_BUFFER_BIT);
    		glMatrixMode(GL_MODELVIEW); 
    		glLoadIdentity();

/* _________________ Dessin du sous-menu pour choisir la carte( qui marche pas )_______________*/
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, MapMenu);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(300,300,1);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

/* _________________ Dessin du bouton d'aide_______________*/

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, helpButton);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(300,300,1);
		glTranslatef(1,-0.5,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

/* _________________ Dessin du bouton d'aide_______________*/

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mapButton);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(300,300,1);
		glTranslatef(-0.5,-0.5,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

}




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
	font = strcat(font,"font/Champagne.ttf");

	police = TTF_OpenFont(font,100);

	SDL_Color blanc = {255, 255, 255};

	SDL_Surface* text = NULL;



	text = TTF_RenderText_Blended(police, "Marin",blanc);
	if (text == NULL){
		printf("error");
	}


	return text;
	
	
}


