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

void drawMenu(GLuint helpButton,GLuint mapButton,GLuint playButton,GLuint casevide,GLuint casechecked, GLuint Bienvenue,GLuint ChoixLegend,GLuint PlayLegend,GLuint ReglesLegend,GLuint regles,GLuint bulle, GLuint carteMarine,GLuint carteSpatiale,  int* menuOpen,int* aideOpen,int* playIsPush, char* mapName){


		glClear(GL_COLOR_BUFFER_BIT);
    		glMatrixMode(GL_MODELVIEW); 
    		glLoadIdentity();

/*______________Bienvenue____________________________________*/
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Bienvenue);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(0,204,204);
		glRotatef(180,0,0,1);
		glScalef(1300,150,1);
		glTranslatef(0,-1.7,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);



/*______________Choix____________________________________*/
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ChoixLegend);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(0,204,204);
		glRotatef(180,0,0,1);
		glScalef(400,100,1);
		glTranslatef(-0.4,-0.1,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
	



/*______________PLay !____________________________________*/
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, PlayLegend);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(0.,204.,204.);
		glRotatef(180.,0.,0.,1.);
		glScalef(300.,100.,1.);
		glTranslatef(0.,2.,0.);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

/*______________Règles____________________________________*/
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,ReglesLegend);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(0.,204.,204.);
		glRotatef(180.,0.,0.,1.);
		glScalef(400.,100.,1.);
		glTranslatef(0.3,-0.1,0.);
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
		glScalef(400,400,1);
		glTranslatef(0.3,-0.3,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

/* _________________ Dessin du bouton pour choisir la carte_______________*/

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mapButton);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(400,400,1);
		glTranslatef(-0.3,-0.3,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

/* _________________ Dessin du bouton play_______________*/

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, playButton);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(400,400,1);
		glTranslatef(0,0.3,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);



		if (*menuOpen == 1){
			drawMapMenu(carteMarine,carteSpatiale,casevide,casechecked, mapName);
		}

		if (*aideOpen == 1){
		/*______________Règles____________________________________*/
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, regles);

		
			glPushMatrix();
			glAlphaFunc(GL_GREATER,0.0f);
			glColor3ub(255,255,255);
			glRotatef(180,0,0,1);
			glScalef(1500,1500,1);
			glTranslatef(0,0,0);
			drawButton();
			glPopMatrix();

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);

		}

		if (*playIsPush == 1){
			if (strcmp(mapName,"Not chosen")==0){
				/*dessin de la bulle */
				glEnable(GL_ALPHA_TEST);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, bulle);

		
				glPushMatrix();
				glAlphaFunc(GL_GREATER,0.0f);
				glColor3ub(255,255,255);
				glRotatef(180,0,0,1);
				glScalef(700,700,1);
				glTranslatef(-0.2,0.15,0);
				drawButton();
				glPopMatrix();

				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_ALPHA_TEST);
			}
			else{
				*playIsPush =2;
			}
		}

}

void drawMapMenu (GLuint carteMarine,GLuint carteSpatiale, GLuint casevide,GLuint casechecked, char* mapName){

/* _________________ Dessin du sous-menu pour choisir la carte_______________*/
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, carteMarine);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(300,100,1);
		glTranslatef(-0.8,-1.2,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

	/*_______________________________________________________*/
	
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, carteSpatiale);

		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(400,100,1);
		glTranslatef(-0.65,-0.8,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

/* _________________ Dessin de la case vide 1_______________*/

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, casevide);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(100,100,1);
		glTranslatef(-3.5,-1.2,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

/* _________________ Dessin de la case vide 2_______________*/

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, casevide);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(100,100,1);
		glTranslatef(-3.7,-0.8,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

/* _________________ check de la case 1_______________*/

		if (strcmp(mapName,"map1")==0){

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, casechecked);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(100,100,1);
		glTranslatef(-3.5,-1.2,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
	}

/* _________________ check de la case 2_______________*/

		if (strcmp(mapName,"map2")==0){

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, casechecked);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(100,100,1);
		glTranslatef(-3.7,-0.8,0);
		drawButton();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
	}


}






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


SDL_Surface* loadFont(TTF_Font* police, char* str, char* font, int taille){
	
	

	police = TTF_OpenFont(font,taille);

	SDL_Color blanc = {255, 255, 255};

	SDL_Surface* text = NULL;



	text = TTF_RenderText_Blended(police,str,blanc);
	if (text == NULL){
		printf("error");
	}


	return text;
	
	
}


