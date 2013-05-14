#ifndef ITD_INTERFACE_DRAWER
#define ITD_INTERFACE_DRAWER
#include <SDL/SDL_ttf.h>
#include "graphics.h"
#include "geometry.h"
#include "list.h"

/**
 * L'interface contient l'ensemble des boutons nécessaires à l'interaction
 * avec le jeu. L'interface contient un ensemble de boutons sur lequel l'utilisateur peut cliquer.
 * Elle contient aussi la somme d'argent restante au joueur ainsi qu'un panneau d'informations
 * sur la tour sélectionnée par le joueur.
 * ATTENTION : Les paramètres envoyés à initInterface travaillent en repère SDL.
 * Rappel : le repère OpenGL a comme unité le pixel, tout comme le repère SDL.
 */
 
typedef enum{
	CLICK_ON_MAP, CLICK_ON_TOWER, PUT_LASER, PUT_GUN, PUT_ROCKET, PUT_HYBRID, QUIT_GAME, PAUSE_GAME, NO_ACTION, AIDE_MENU,CHOIX_MENU,PLAY_MENU,MAP_MENU,MAP_MENU_CASE,CLOSE_RULES_MENU
}Action;

/**
 * Un bouton est caractérisé par l'action qu'il déclenche
 * sa position (position du coin haut gautche) ainsi que sa taille.
 */
typedef struct{
	Action action;///Action déclenchée par le bouton
	Point3D position;///Position du coin haut gauche
	float width;///En pixels
	float height;///En pixels
}Button;

typedef struct{
	Button* regles;
	Button* jouer;
	Button* choix_carte;
	Button* close_rules;
	Button* carte[NB_MAX_MAPS];
	Button* cases[NB_MAX_MAPS];
	int cmp;
	char* tabMapName[NB_MAX_MAPS];
}ButtonOfMenu;

ButtonOfMenu BUTTON_OF_MENU;	

/**
 * Structure représentant l'interface du jeu.
 */
typedef struct{
	float width;///En pixels
	float height;///En pixels
	Point3D position;///Position du milieu de l'interface (coordonnées OpenGL)
	List* lstButtons;///Liste des boutons que contient l'interface
	float moneyWidth;///Largeur de la surface affichant l'argent (en px)
	float moneyHeight;///Hauteur de la surface affichant l'argent (en px)
	Point3D moneyPosition;///Position du milieu de la surface affichant l'argent
	float infoWidth;
	float infoHeight;
	Point3D infoPosition;
	Action currentAction;///L'action en cours du joueur suite à une interaction sur l'interface
}Interface;

/**
 * Initialise la liste des boutons contenus dans l'interface.
 * Attention : la largeur, la hauteur, la positionX et la positionY doivent être un pourcentage 
 * de la taille de la fenêtre. Les valeurs non comprises entre 0 et 1.
 * Le positionnement se fait par rapport au coin haut gauche de l'interface.
 * Le pourcentage de la position prend comme origine le repère SDL.
 * seront taillées pour être adaptées.
 */
Interface initGameInterface(float width, float height, float positionX, float positionY);

/**
 * Dessine l'interface pointée par interface.
 */
void drawInterface(Interface* interface);
SDL_Surface* loadFont(TTF_Font*, char* str,char* rootPath, int taille);
void drawMenu(GLuint*,int, int*,int*,int*, char*);
void drawMapMenu (char* mapName);

/** 
  *Met à jour la texture affichant l'argent du joueur.
  */
void updateMoneyTexture(Interface* interface, int money);

/**
 * Met à jour la texture affichant les informations sur 
 * une tour.
 */
void updateInfoTexture(Interface* interface, char* name, int power, int rate, int range);

//Fonctions internes
Button* createButton(Action action, Point3D position, float width, float height);
void drawButtonMenu();
void drawButton(const Button* button);
void drawCarre();

#endif
