#ifndef ITD_ACTION_MANAGER_H
#define ITD_ACTION_MANAGER_H
#include "utils.h"
#include <SDL/SDL.h>
#include "world.h"
#include "interfaceDrawer.h"


void handleMenuActions(char* mapName);


bool handleGameActions(World* world, Interface* interface);

//Fonctions internes
bool handleGameKeyboard(const SDL_KeyboardEvent* e, World* world);
bool handleGameMouse(const SDL_Event* e, World* world);

void clicButton (SDL_Event e);

#endif
