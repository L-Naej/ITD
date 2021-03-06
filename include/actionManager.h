#ifndef ITD_ACTION_MANAGER_H
#define ITD_ACTION_MANAGER_H
#include "utils.h"
#include <SDL/SDL.h>
#ifdef MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "world.h"
#include "interfaceDrawer.h"

//Temps qui s'écoule entre la fin du jeu et le retour au menu en millisecondes
//(permet d'afficher un message de fin de jeu)
static const Uint32 END_GAME_DURATION = 3000;
/**
 * Gère les actions clavier et souris du joueur lorsqu'il
 * est dans le menu d'accueil.
 * Renvoie true si le joueur a demandé à quitter le programme.
 */	
bool handleMenuActions(char** mapName,int* playIsPush, int* menuOpen,int* aideOpen);

/**
 * Gère les actions clavier et souris du joueur lorsqu'il
 * est dans la phase de jeu.
 * Renvoie true si le joueur a demandé à quitter le programme.
 */
bool handleGameActions(World* world, Interface* interface, bool* gameIsFinished);

//Fonctions internes
void scrollMenu(int direction);
bool handleGameKeyboard(const SDL_KeyboardEvent* e, World* world, Interface* interface);
bool handleGameMouse(const SDL_Event* e, World* world, Interface* interface);
Action detectAction(Uint16 x, Uint16 y, World* world, Interface* interface, Tower** pointedTower);
void suppressTower(World* world, Uint16 x, Uint16 y);
bool isMouseOnInterface(Uint16 x, Uint16 y, Interface* interface);
bool isMouseOnButton(Button* button, Uint16 x, Uint16 y);
bool isMouseOnTower(Tower* tower, Point3D cameraPosition, Uint16 x, Uint16 y);

void clicButton (SDL_Event e,int* playIsPush, float x, float y, int* menuOpen,int* aideOpen, char** mapName);


#endif
