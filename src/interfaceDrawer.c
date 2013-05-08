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
#include <math.h>
#include "interfaceDrawer.h"
#include "world.h"

Button* createButton(Action action, Point3D position, float width, float height){
	Button* button = (Button*) malloc(sizeof(Button));
	if(button == NULL){
		fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire nécessaire, le programme va quitter.\n");
		exit(-1);
	}
	
	button->action = action;
	button->position = position;
	button->width = width;
	button->height = height;
	return button;
}

Interface initGameInterface(float width, float height, float positionX, float positionY){
	//Vérif des paramètres
	if(width > 1.0 ) width = 1.0;
	else if (width < 0.0 ) width = 0.0;
	if(height > 1.0 ) height = 1.0;
	else if (height < 0.0 ) height = 0.0;
	if(positionX > 1.0 ) positionX = 1.0;
	else if (positionX < 0.0 ) positionX = 0.0;
	if(positionY > 1.0 ) positionY = 1.0;
	else if (positionY < 0.0 ) positionY = 0.0;
	
	Interface interface;
	
	interface.lstButtons = NULL;
	interface.currentAction = NO_ACTION;
	
	//Calcul des dimensions de l'interface
	interface.width = WINDOW_WIDTH * width;
	interface.height = WINDOW_HEIGHT * height;
	Point3D sdlPosition = PointXYZ(WINDOW_WIDTH *positionX, WINDOW_HEIGHT *positionY, 0.0);
	interface.position = sdlToOpenGL(sdlPosition);
	
	if(TTF_Init() == -1){
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	TTF_Font* police = NULL;
	police = TTF_OpenFont("lighthouse.ttf", 100);
	
	//Création de l'espace pour dessiner l'argent restant
	SDL_Color color = {255,255,255};	
	interface.panelMoney = TTF_RenderText_Blended(police, "TESTTESTEST", color);
	interface.moneyTexture = makeTextureFromSurface(interface.panelMoney);
	
	TTF_CloseFont(police);
	TTF_Quit();
	
	//Création des boutons 
	List* lstButtons = createEmptyList();
	if(lstButtons == NULL){
		fprintf(stderr, "Erreur fatale : Impossible d'allouer la mémoire nécessaire, le programme va quiter.\n");
		exit(-1);
	}
	
	//Interface horizontale ou verticale ?
	float buttonWidth = 0.0;
	float buttonHeight = 0.0;
	float xStep = 0.0, yStep = 0.0;
	//Horizontale
	if(interface.width > interface.height){
		buttonWidth = interface.width / 2.0 * 0.1;
		buttonHeight = interface.height / 2.0 * 0.95;
		xStep = buttonWidth + interface.width * 0.05;
		yStep = 0.0;
	}
	//Verticale
	else{ 
		buttonWidth = interface.width / 2.0 * 0.95;
		buttonHeight = interface.height / 2.0 * 0.1;
		xStep = 0.0;
		yStep = buttonHeight + interface.height * 0.05;
	}

	Button* btnLaser = createButton(PUT_LASER, PointXYZ(interface.position.x, interface.position.y, 0.0), buttonWidth, buttonHeight);
	Button* btnGun = createButton(PUT_GUN, PointXYZ(interface.position.x + xStep, interface.position.y - yStep, 0.0), buttonWidth, buttonHeight);
	Button* btnRocket = createButton(PUT_ROCKET, PointXYZ(interface.position.x + xStep*2, interface.position.y - yStep*2, 0.0), buttonWidth, buttonHeight);
	Button* btnHybrid = createButton(PUT_HYBRID, PointXYZ(interface.position.x + xStep*3, interface.position.y - yStep*3, 0.0), buttonWidth, buttonHeight);
	Button* btnQuit = createButton(QUIT_GAME, PointXYZ(interface.position.x + xStep*4, interface.position.y - yStep*4, 0.0), buttonWidth, buttonHeight);
	
	insertBottomCell(lstButtons, btnLaser);
	insertBottomCell(lstButtons, btnGun);
	insertBottomCell(lstButtons, btnRocket);
	insertBottomCell(lstButtons, btnHybrid);
	insertBottomCell(lstButtons, btnQuit);
	
	interface.lstButtons = lstButtons;
	return interface;
}

//TODO
void drawInterface(const Interface* interface){
	if(interface == NULL) return;
	
	glLoadIdentity();
	//Dessin du fond de l'interface
	glColor3ub(0,0,0);
	
	glPushMatrix();
	glTranslatef(interface->position.x + interface->width / 2.0, interface->position.y - interface->height / 2.0, interface->position.z);
	
	glScalef(interface->width, interface->height, 1.0);
	drawQuad();
	
	glPopMatrix();
	
	//Dessin du texte argent
	glPushMatrix();
	glLoadIdentity();
	glColor3ub(255,255,255);
	
	glEnable(GL_ALPHA_TEST);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, interface->moneyTexture);
	//glPushMatrix();
	glAlphaFunc(GL_GREATER,0.0f);
	glColor3ub(255,255,255);
	glRotatef(180,0,0,1);
	glScalef(300,300,1);
	drawTexturedQuad(interface->moneyTexture);
	glPopMatrix();

	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
    
	//glScalef(interface->panelMoney->w,interface->panelMoney->h,1);
	//drawTexturedQuad(textureId);
	//glDeleteTextures(1, &textureId);
	
	//glPopMatrix();
	//Dessin des boutons
	glColor3ub(255,255,255);
	goToHeadList(interface->lstButtons);
	Button* cur = NULL;
	while( (cur = (Button*)nextData(interface->lstButtons) ) != NULL){
		drawButton(cur);
	}
	
	//Dessin des infos sur une tour cliquée (si une tour a été cliquée)
	
	//Dessin d'une tour sur la souris si l'action courante est de poser une tour
	GLuint textureId = 0;
	bool drawUnderMouse = true;
	//TODO
	switch(interface->currentAction){
	case PUT_GUN : textureId = GAME_TEXTURES_ID.GUN_TOWER_ID;
	break;
	case PUT_HYBRID : textureId = GAME_TEXTURES_ID.HYBRID_TOWER_ID;
	break;
	case PUT_LASER : textureId = GAME_TEXTURES_ID.LASER_TOWER_ID;
	break;
	case PUT_ROCKET: textureId = GAME_TEXTURES_ID.ROCKET_TOWER_ID;
	break;
	case NO_ACTION : drawUnderMouse = false;
	break;
	case QUIT_GAME : drawUnderMouse = false;
	break;
	default : drawUnderMouse = false;
	}
	
	if(drawUnderMouse){
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		Point3D oglMouse = sdlToOpenGL(PointXYZ(mouseX,mouseY, 0.0));
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(oglMouse.x, oglMouse.y, oglMouse.z);
		glScalef(TOWER_WIDTH_PX,TOWER_HEIGHT_PX, 1.0);
		drawTexturedQuad(textureId);
		glPopMatrix();
	}
	
}

void drawMenu(){}

void drawButton(const Button* button){
	if(button == NULL) return;
	GLuint textureId = 0;
	
	switch(button->action){
	case PUT_GUN : textureId = GAME_TEXTURES_ID.GUN_TOWER_ID;
	break;
	case PUT_HYBRID : textureId = GAME_TEXTURES_ID.HYBRID_TOWER_ID;
	break;
	case PUT_LASER : textureId = GAME_TEXTURES_ID.LASER_TOWER_ID;
	break;
	case PUT_ROCKET : textureId = GAME_TEXTURES_ID.ROCKET_TOWER_ID;
	break;
	case QUIT_GAME : textureId = GAME_TEXTURES_ID.QUIT_GAME_ID;
	break;
	default : return;
	break;
	}
	
	glPushMatrix();
	
	glTranslatef(button->position.x + button->width / 2.0, button->position.y - button->height / 2.0, 0.0);
	glScalef(button->width, button->height,1.);
	drawTexturedQuad(textureId);
	glPopMatrix();
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
	
	police = TTF_OpenFont("lighthouse.ttf",1);

	SDL_Color blanc = {255, 255, 255};

	SDL_Surface* text = NULL;



	text = TTF_RenderText_Solid(police, "Coucou! Salut, blabla",blanc);
	if (text == NULL){
		printf("error");
	}


	return text;
}
