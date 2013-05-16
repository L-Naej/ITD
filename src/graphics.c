#include "graphics.h"
#include <SDL/SDL_image.h>
#include <math.h>
#define CIRCLE_PRECISION 100

//TODO
void initGameGraphics(SDL_Surface* map){
	if(map == NULL){
		fprintf(stderr, "Erreur : les graphismes du jeu ne peuvent être initialisés avant le chargement de la map.\n");
		exit(-1);
	}
	GLuint tab[] = {GAME_TEXTURES_ID.BLUE_OCTOPUS_ID, 
	GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID,  
	GAME_TEXTURES_ID.GREEN_OCTOPUS_ID, 
	GAME_TEXTURES_ID.LASER_TOWER_ID, 
	GAME_TEXTURES_ID.ROCKET_TOWER_ID, 
	GAME_TEXTURES_ID.GUN_TOWER_ID, 
	GAME_TEXTURES_ID.HYBRID_TOWER_ID, 
	GAME_TEXTURES_ID.QUIT_GAME_ID, 
	GAME_TEXTURES_ID.MONEY_ID, 
	GAME_TEXTURES_ID.WAVE_MESSAGE_ID, 
	GAME_TEXTURES_ID.PAUSE_MESSAGE_ID, 
	GAME_TEXTURES_ID.INFO_PANEL_ID, 
	GAME_TEXTURES_ID.WIN_MESSAGE_ID };
	glDeleteTextures(13,tab);
	
	
	GAME_TEXTURES_ID.BLUE_OCTOPUS_ID = makeTextureFromFile("images/monster1face.png");
	GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID = makeTextureFromFile("images/monster2face.png");
	GAME_TEXTURES_ID.GREEN_OCTOPUS_ID = makeTextureFromFile("images/monster3face.png");
	GAME_TEXTURES_ID.LASER_TOWER_ID = makeTextureFromFile("images/laser.png");
	GAME_TEXTURES_ID.ROCKET_TOWER_ID = makeTextureFromFile("images/rocket.png");
	GAME_TEXTURES_ID.GUN_TOWER_ID = makeTextureFromFile("images/mitraillette.png");
	GAME_TEXTURES_ID.HYBRID_TOWER_ID = makeTextureFromFile("images/hybrid.png");
	GAME_TEXTURES_ID.QUIT_GAME_ID = makeTextureFromFile("images/monstrehelp.png");
	
	//Initialisées après par l'interace
	GAME_TEXTURES_ID.MONEY_ID = 0;
	GAME_TEXTURES_ID.WAVE_MESSAGE_ID = 0;
	GAME_TEXTURES_ID.PAUSE_MESSAGE_ID = 0;
	GAME_TEXTURES_ID.INFO_PANEL_ID = 0;
	GAME_TEXTURES_ID.WIN_MESSAGE_ID = 0;
		
	GAME_TEXTURES_ID.MAP_ID = makeTextureFromSurface(map);
}

GLuint makeTextureFromFile(const char* imagePath){
	if(imagePath == NULL) return 0;
	
	GLuint textureId;
	
	SDL_Surface* image = IMG_Load(imagePath);
	if (image ==NULL){
		fprintf(stderr,"Erreur fatale : impossible de charger l'image %s\n", imagePath);
		exit(1);
	}

	textureId = makeTextureFromSurface(image);
	
	SDL_FreeSurface(image);
	
	return textureId;
}

GLuint makeTextureFromSurface(SDL_Surface* image){

	if(image == NULL) return 0;
	GLuint textureId;
	GLenum format;
	glEnable(GL_TEXTURE_2D);	
	glGenTextures(1,&textureId);

	glBindTexture(GL_TEXTURE_2D,textureId);

	Uint8 nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
	if (image->format->Rmask == 0x000000ff)
		format = GL_RGBA;
	else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
	if (image->format->Rmask == 0x000000ff)
		format = GL_RGB;
	else
		format = GL_BGR;
	} else {
	printf("warning: the image is not truecolor..  this will probably break\n");
	// this error should not go unhandled
	}

	/*envoi de la texture à openGL*/
	//SDL_LockSurface(image);
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, image->w, image->h, 0, format,GL_UNSIGNED_BYTE, image->pixels);
	//SDL_UnlockSurface(image);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);	
	return textureId;
}

void updateTextureFromSurface(GLuint textureId, SDL_Surface* surface){
	if(surface == NULL) return;
	GLenum format;
	Uint8 nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
	if (surface->format->Rmask == 0x000000ff)
		format = GL_RGBA;
	else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
	if (surface->format->Rmask == 0x000000ff)
		format = GL_RGB;
	else
		format = GL_BGR;
	} else {
	printf("warning: the image is not truecolor..  this will probably break\n");
	// this error should not go unhandled
	}

	glEnable(GL_TEXTURE_2D);	
	glBindTexture(GL_TEXTURE_2D, textureId);
	SDL_LockSurface(surface);
	glTexImage2D(GL_TEXTURE_2D,0, nOfColors, surface->w, surface->h, 0, format,GL_UNSIGNED_BYTE, surface->pixels);
	SDL_UnlockSurface(surface);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);	
}

GLuint makeTextureFromSurfaces(SDL_Surface** surfaces, int nbSurfaces, int alignement, int*width, int* height){
	if(surfaces == NULL || nbSurfaces <= 0 || width == NULL || height == NULL) return 0;
	int i = 0;
	int HORIZONTAL = 0, VERTICAL = 1;
	int totalWidth = 0, totalHeight = 0, tmp = 0;
	GLuint textureId;

	
	//On calcule la taille globale de la texture
	for(i = 0; i < nbSurfaces; ++i){
		if(alignement == HORIZONTAL){
			totalWidth += surfaces[i]->w;
			tmp = surfaces[i]->h;
			if(totalHeight < tmp) totalHeight = tmp;
		}
		else if(alignement == VERTICAL){
			totalHeight += surfaces[i]->h;
			tmp = surfaces[i]->w;
			if(totalWidth < tmp) totalWidth = tmp;
		}
	}
	
	glEnable(GL_TEXTURE_2D);	
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	//On créé la texture contenant l'ensemble des surfaces, pour l'instant vide 
	SDL_Surface* black = SDL_CreateRGBSurface(SDL_HWSURFACE, totalWidth, totalHeight, 32, 0, 0, 0, 0);
	Uint32 color = SDL_MapRGBA(black->format, 0, 0, 0,0);
	SDL_FillRect(black, NULL, color);
	SDL_LockSurface(black);
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, totalWidth, totalHeight, 0, GL_RGBA,GL_UNSIGNED_BYTE, black->pixels);
	SDL_UnlockSurface(black);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	int xOffset = 0, yOffset = 0;
	//On ajoute les surfaces à la texture
	for(i = 0; i < nbSurfaces; ++i){
	
		GLenum format;
		Uint8 nOfColors = surfaces[i]->format->BytesPerPixel;
		if (nOfColors == 4)     // contains an alpha channel
		{
		if (surfaces[i]->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
		    	format = GL_BGRA;
		} else if (nOfColors == 3)     // no alpha channel
		{
		if (surfaces[i]->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
		} else {
		printf("warning: the image is not truecolor..  this will probably break\n");
		// this error should not go unhandled
		}
		SDL_LockSurface(surfaces[i]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, surfaces[i]->w, surfaces[i]->h, format, GL_UNSIGNED_BYTE, surfaces[i]->pixels);
		SDL_UnlockSurface(surfaces[i]);
		if(alignement == HORIZONTAL) xOffset += surfaces[i]->w;
		else yOffset += surfaces[i]->h;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	*width = totalWidth;
	*height = totalHeight;
	return textureId;
}

void drawQuad(){
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	
	//Bottom left
	glVertex2f(-0.5,-0.5);
	//Bottom right
	glVertex2f(0.5,-0.5);

	//Top right
	glVertex2f(0.5,0.5);
	//Top left
	glVertex2f(-0.5, 0.5);

	glEnd();
	glDisable(GL_BLEND);
}

void drawTexturedQuad(GLuint textureId){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	
	//Bottom left
	glTexCoord2f(0.0,1.0);
	glVertex2f(-0.5,-0.5);
	//Bottom right
	glTexCoord2f(1.0,1.0);
	glVertex2f(0.5,-0.5);

	//Top right
	glTexCoord2f(1.0,0.0);
	glVertex2f(0.5,0.5);
	//Top left
	glTexCoord2f(0.0,0.0);
	glVertex2f(-0.5, 0.5);

	glEnd();
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
}

void drawCircle(int full){
	float anglePrecision = 2*M_PI / CIRCLE_PRECISION;
	int i = 0;

	//y = 1 - x²
	if(full) glBegin(GL_POLYGON);
	else glBegin(GL_LINE_LOOP);
	for(i = 0; i < CIRCLE_PRECISION; ++i){
		glVertex2f(cos(i*anglePrecision), sin(i*anglePrecision));
		
	}
	glEnd();
	

}

//Considère la caméra en 0;0;0
Point3D openGLToItd(int mapWidth, int mapHeight, Point3D oglPoint){
	Point3D itdPoint;
	itdPoint.z = 0.0;
	itdPoint.x = oglPoint.x + mapWidth / 2;
	itdPoint.y = oglPoint.y + mapHeight / 2;
	itdPoint. y = mapHeight - itdPoint.y;	
	return itdPoint;
}

//Considère la caméra en 0;0;0
Point3D itdToOpenGL(int mapWidth, int mapHeight, Point3D itdPoint){
	Point3D oglPoint;
	float maxWidth, maxHeight;
	maxWidth = mapWidth;
	maxHeight = mapHeight;
	oglPoint.x = -(maxWidth / 2.0) + maxWidth * itdPoint.x / maxWidth; 	
	oglPoint.y = ((maxHeight / 2.0) - maxHeight * itdPoint.y / maxHeight); 
	oglPoint.z = itdPoint.z;

	return oglPoint;	
}

//Considère la caméra en 0;0;0
Point3D sdlToOpenGL(Point3D sdlPoint){
	Point3D oglPoint;
	oglPoint.z = sdlPoint.z;
	
	oglPoint.x = GL_X_AXIS_MIN + 2*GL_X_AXIS_MAX* sdlPoint.x / WINDOW_WIDTH;
	oglPoint.y = - (GL_Y_AXIS_MIN + 2*GL_Y_AXIS_MAX* sdlPoint.y / WINDOW_HEIGHT);
	
	return oglPoint;
}

void reshape() {
	glViewport(0, 0 , WINDOW_WIDTH , WINDOW_HEIGHT );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GL_X_AXIS_MAX = WINDOW_WIDTH / 2.0;
	GL_X_AXIS_MIN = - GL_X_AXIS_MAX;
	GL_Y_AXIS_MAX = WINDOW_HEIGHT / 2.0;
	GL_Y_AXIS_MIN = - GL_Y_AXIS_MAX;

	gluOrtho2D(GL_X_AXIS_MIN, GL_X_AXIS_MAX, GL_Y_AXIS_MIN, GL_Y_AXIS_MAX);
}

int setVideoMode(int w, int h) {
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
	if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
		return EXIT_FAILURE;
	}

	reshape();

	return EXIT_SUCCESS;
}

int initWindow(){
	/* Initialisation de la SDL */
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
	fprintf(stderr, "Impossible d'initialiser la SDL.\n");
	return EXIT_FAILURE;
	}

	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	if(EXIT_FAILURE == setVideoMode(800, 600)) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre.\n");
		return EXIT_FAILURE;
	}

	/* Titre de la fenêtre */
	SDL_WM_SetCaption("ITD Avatanéo Camarasa Chiganne", NULL);
	SDL_WarpMouse((Uint16)(WINDOW_WIDTH / 2), (Uint16)(WINDOW_HEIGHT/2));
	return 0;
}
