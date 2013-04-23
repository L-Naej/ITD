#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "interfaceDrawer.h"
//TODO
void drawInterface(){}
void drawMenu(){}


void drawCarre(){
	glBegin(GL_POLYGON);
 	glTexCoord2f(0, 1);
    	glVertex2f(-0.5,-0.5);
 	glTexCoord2f(1, 1);
    	glVertex2f(-0.5,0.5);
 	glTexCoord2f(1, 0);
    	glVertex2f(0.5,0.5);
 	glTexCoord2f(0, 0);
    	glVertex2f(0.5,-0.5);
    glEnd();
}

GLuint makeTexture (void){
	GLuint texture;
	GLenum format;
	char* menuHelp = "images/monstrehelp.png";
	SDL_Surface* image = IMG_Load(menuHelp);
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
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, image->w, image->h, 0, format,GL_UNSIGNED_BYTE, image->pixels); 
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image);
}
