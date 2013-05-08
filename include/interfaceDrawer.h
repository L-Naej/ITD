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
	CLICK_ON_MAP, PUT_LASER, PUT_GUN, PUT_ROCKET, PUT_HYBRID, QUIT_GAME, NO_ACTION
}Action;

/**
 * Un bouton est caractérisé par l'action qu'il déclenche
 * sa position (position du coin haut gautche) ainsi que sa taille.
 */
typedef struct{
	Action action;
	Point3D position;///Position du coin haut gauche
	float width;///En pixels
	float height;///En pixels
}Button;
	

/**
 * Structure représentant l'interface du jeu.
 */
typedef struct{
	float width;///En pixels
	float height;///En pixels
	Point3D position;///Position du coin haut gauche de l'interface (coordonnées OpenGL)
	List* lstButtons;
	SDL_Surface* panelMoney;
	SDL_Surface* panelInfo;
	Action currentAction;
}Interface;

/**
 * Initialise la liste des boutons contenus dans l'interface.
 * Attention : la largeur, la hauteur, la positionX et la positionY doivent être un pourcentage 
 * de la taille de la fenêtre. Les valeurs non comprises entre 0 et 1.
 * Le positionnement se fait par rapport au coin haut gauche de l'élément.
 * Le pourcentage de la position prend comme origine le repère SDL.
 * seront taillées pour être adaptées.
 */
Interface initGameInterface(float width, float height, float positionX, float positionY);

void drawInterface(const Interface* interface);
void drawMenu();

//Fonctions internes
Button* createButton(Action action, Point3D position, float width, float height);

void drawButton(const Button* button);
void drawCarre(void);
SDL_Surface* drawMapMenu(TTF_Font*);

#endif
