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

void drawButtonMenu(){
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

void drawMenu(GLuint helpButton,GLuint MapMenu,GLuint mapButton){
		glClear(GL_COLOR_BUFFER_BIT);
    		glMatrixMode(GL_MODELVIEW); 
    		glLoadIdentity();

/* _________________ Dessin du sous-menu pour choisir la carte______________*/
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, MapMenu);

		
		glPushMatrix();
		glAlphaFunc(GL_GREATER,0.0f);
		glColor3ub(255,255,255);
		glRotatef(180,0,0,1);
		glScalef(300,300,1);
		drawButtonMenu();
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
		drawButtonMenu();
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
		drawButtonMenu();
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);

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
	interface.moneyPosition = interface.position;

	//On positionne en fonction du coin haut gauche
	interface.position.x += interface.width / 2.0;
	interface.position.y -= + interface.height / 2.0;
	 
	//Création des textures affichant du texte
	if(TTF_Init() == -1){
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	TTF_Font* police = NULL;
	police = TTF_OpenFont("font/Champagne.ttf", 40);
	
	//Création de l'espace et de la texture pour dessiner l'argent restant
	SDL_Color color = {255,255,255};	
	SDL_Surface* moneySurface = TTF_RenderText_Blended(police, "00000", color);
	GAME_TEXTURES_ID.MONEY_ID = makeTextureFromSurface(moneySurface);
	interface.moneyWidth = moneySurface->w > interface.width ? 0.9*interface.width : moneySurface->w;
	interface.moneyHeight = moneySurface->h;
	interface.moneyPosition.y = interface.moneyPosition.y - interface.moneyHeight / 2.0;
	interface.moneyPosition.x = interface.moneyPosition.x + interface.moneyWidth / 2.0;
	
	interface.infoPosition.z = interface.position.z;
	
	//Création de la texture affichant le message "Pause"
	SDL_Color colorPause = {255,0,0};
	SDL_Surface* pauseSurface = TTF_RenderText_Blended(police, "PAUSE", colorPause);
	GAME_TEXTURES_ID.PAUSE_MESSAGE_ID = makeTextureFromSurface(pauseSurface);
	
	//Création de la texture contenant les informations
	SDL_Surface* infoSurface = TTF_RenderText_Blended(police, "IMAC ITD", color);
	GAME_TEXTURES_ID.INFO_PANEL_ID = makeTextureFromSurface(infoSurface);
	interface.infoWidth = infoSurface->w;
	interface.infoHeight = infoSurface->h;
	//La position du panel d'info est déterminée après que les boutons ait été créés
	
	SDL_FreeSurface(infoSurface);
	SDL_FreeSurface(pauseSurface);
	SDL_FreeSurface(moneySurface);
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
	Point3D buttonStart; 
	//Horizontale
	if(interface.width > interface.height){
		buttonWidth = interface.width / 2.0 * 0.1;
		buttonHeight = interface.height / 2.0 * 0.95;
		interface.infoHeight = interface.infoHeight > interface.height ? interface.height : interface.infoHeight;
		xStep = buttonWidth + interface.width * 0.05;
		yStep = 0.0;
		buttonStart = PointXYZ(interface.moneyPosition.x + interface.moneyWidth + buttonWidth / 2.0, interface.moneyPosition.y - buttonHeight / 2.0, 0.0); 
	}
	//Verticale
	else{ 
		buttonWidth = interface.width / 2.0 * 0.70;
		buttonHeight = interface.height / 2.0 * 0.1;
		interface.infoWidth = interface.infoWidth > interface.width ? interface.width : interface.infoWidth;
		xStep = 0.0;
		yStep = buttonHeight + interface.height * 0.05;
		buttonStart = PointXYZ(interface.moneyPosition.x, interface.moneyPosition.y - interface.moneyHeight - buttonHeight / 2.0, 0.0);
	}
	
	Button* btnLaser = createButton(PUT_LASER, PointXYZ(buttonStart.x , buttonStart.y, 0.0), buttonWidth, buttonHeight);
	Button* btnGun = createButton(PUT_GUN, PointXYZ(buttonStart.x + xStep,  buttonStart.y - yStep, 0.0), buttonWidth, buttonHeight);
	Button* btnRocket = createButton(PUT_ROCKET, PointXYZ(buttonStart.x + xStep*2,  buttonStart.y - yStep*2, 0.0), buttonWidth, buttonHeight);
	Button* btnHybrid = createButton(PUT_HYBRID, PointXYZ(buttonStart.x + xStep*3,  buttonStart.y - yStep*3, 0.0), buttonWidth, buttonHeight);
	Button* btnQuit = createButton(QUIT_GAME, PointXYZ(buttonStart.x + xStep*4,  buttonStart.y - yStep*4, 0.0), buttonWidth, buttonHeight);
	
	if(interface.width > interface.height){
		Point3D droiteInterface;
		droiteInterface.x = interface.position.x + interface.width / 2.0;
		droiteInterface.y = interface.position.y;
		Point3D droiteBouton;
		droiteBouton.x = btnQuit->position.x + buttonWidth / 2.0;
		droiteBouton.y = btnQuit->position.y;
		interface.infoPosition.y = interface.position.y;
		interface.infoPosition.x = droiteInterface.x - fabs(droiteInterface.x - droiteBouton.x) /2.0;
	}
	else{
		Point3D basInterface;
		basInterface.x = interface.position.x;
		basInterface.y = interface.position.y - interface.height / 2.0;
		Point3D basBouton;
		basBouton.x = interface.position.x;
		basBouton.y = btnQuit->position.y - buttonHeight / 2.0;
		interface.infoPosition.y = basInterface.y + fabs(basInterface.y - basBouton.y) / 2.0;
		interface.infoPosition.x = interface.position.x;
	}

	insertBottomCell(lstButtons, btnLaser);
	insertBottomCell(lstButtons, btnGun);
	insertBottomCell(lstButtons, btnRocket);
	insertBottomCell(lstButtons, btnHybrid);
	insertBottomCell(lstButtons, btnQuit);
	
	interface.lstButtons = lstButtons;
	return interface;
}

//TODO
void drawInterface(Interface* interface){
	
	if(interface == NULL) return;
	
	glLoadIdentity();
	//Dessin du fond de l'interface
	glColor3ub(0,0,0);
	glPushMatrix();
	glTranslatef(interface->position.x, interface->position.y, interface->position.z);
	glScalef(interface->width, interface->height, 1.0);
	drawQuad();
	glPopMatrix();
	
	//Dessin du texte argent
	glPushMatrix();
	glLoadIdentity();
	glColor3ub(255,255,255);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.0f);
	
	glTranslatef(interface->moneyPosition.x, interface->moneyPosition.y, 0.);
	glScalef(interface->moneyWidth,interface->moneyHeight,1.);
	drawTexturedQuad(GAME_TEXTURES_ID.MONEY_ID);
	glDisable(GL_ALPHA_TEST);
	
	glPopMatrix();
	//Dessin des boutons
	glColor3ub(255,255,255);
	goToHeadList(interface->lstButtons);
	Button* cur = NULL;
	while( (cur = (Button*)nextData(interface->lstButtons) ) != NULL){
		drawButton(cur);
	}
	
	//Dessin des infos sur une tour cliquée (si une tour a été cliquée)//TODO
	glPushMatrix();
	glLoadIdentity();
	glColor3ub(255,255,255);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.0f);
	
	glTranslatef(interface->infoPosition.x, interface->infoPosition.y, 0.);
	glScalef(interface->infoWidth,interface->infoHeight,1.);

	drawTexturedQuad(GAME_TEXTURES_ID.INFO_PANEL_ID);
	glDisable(GL_ALPHA_TEST);
	
	glPopMatrix();
	
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
	
	//Si le jeu est mis en pause
	if(interface->currentAction == PAUSE_GAME){
		glPushMatrix();
		glLoadIdentity();
		glColor3ub(255,255,255);
	
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER,0.0f);
	
		glScalef(WINDOW_WIDTH / 5.0,WINDOW_HEIGHT / 10.0,1.);
		drawTexturedQuad(GAME_TEXTURES_ID.PAUSE_MESSAGE_ID);
		glDisable(GL_ALPHA_TEST);
	
		glPopMatrix();
		
	}
	
	if(drawUnderMouse){
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		Point3D oglMouse = sdlToOpenGL(PointXYZ(mouseX,mouseY, 0.0));
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(oglMouse.x, oglMouse.y, oglMouse.z);
		glScalef(TOWER_WIDTH_PX,TOWER_HEIGHT_PX, 1.0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER,0.0f);
		drawTexturedQuad(textureId);
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();
	}
	
}

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
	glTranslatef(button->position.x , button->position.y , 0.0);
	glScalef(button->width, button->height,1.);
	drawTexturedQuad(textureId);
	glPopMatrix();
}

void updateMoneyTexture(Interface* interface, int money){
	char text[6];
	sprintf(text, "%5d", money);
	
	//Création des textures affichant du texte
	if(TTF_Init() == -1){
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	TTF_Font* police = NULL;
	police = TTF_OpenFont("font/Champagne.ttf", 40);
	
	//Création de l'espace pour dessiner l'argent restant
	SDL_Color color = {255,255,255};	
	SDL_Surface* moneySurface = TTF_RenderText_Blended(police, text, color);
	updateTextureFromSurface(GAME_TEXTURES_ID.MONEY_ID, moneySurface);
	
	SDL_FreeSurface(moneySurface);
	TTF_CloseFont(police);
	TTF_Quit();	
}

void updateInfoTexture(Interface* interface, char* name, int power, int rate, int range){
	
	char* phrases [3];
	int phraseLength = 30;
	int nbPhrases = 3, alignement = 0, i = 0;
	phrases[0] = (char*) calloc(phraseLength,sizeof(char));
	phrases[1] = (char*) calloc(phraseLength,sizeof(char));
	phrases[2] = (char*) calloc(phraseLength,sizeof(char));
	sprintf(phrases[0], "Power:%4d", power);
	sprintf(phrases[1], "Rate:%4d", rate);
	sprintf(phrases[2], "Range:%4d", range);
	
	alignement = 1;
	
	//Création des textures affichant du texte
	if(TTF_Init() == -1){
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	TTF_Font* police = NULL;
	police = TTF_OpenFont("font/Champagne.ttf", 35);
	SDL_Color color = {255,255,255};
	
	SDL_Surface* surfaces[4];
	surfaces[0] = TTF_RenderText_Blended(police, name, color);

	for(i = 0; i < nbPhrases; ++i){
		surfaces[i+1] = TTF_RenderText_Blended(police, phrases[i], color);
	}
	int width, height;
	glDeleteTextures(1, &(GAME_TEXTURES_ID.INFO_PANEL_ID));
	GAME_TEXTURES_ID.INFO_PANEL_ID = makeTextureFromSurfaces(surfaces, 4, alignement, &width, &height);
	
	interface->infoHeight = height;
	interface->infoWidth = width;
	
	//gestion basique de la taille du texte
	if(interface->width > interface->height)
		interface->infoHeight = interface->infoHeight > interface->height ? interface->height * 0.9 : interface->infoHeight;
	else{
		interface->infoWidth = interface->infoWidth > interface->width ? interface->width * 0.9 : interface->infoWidth;
	}
	
	for(i = 0; i < 4; ++i){
		SDL_FreeSurface(surfaces[i]);
	}
	
	TTF_CloseFont(police);
	TTF_Quit();	
}

extern char* rootPath;


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


