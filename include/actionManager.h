#ifndef ITD_ACTION_MANAGER_H
#define ITD_ACTION_MANAGER_H
#include "utils.h"
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

void handleMenuActions(char* mapName,int* playIsPush,  int* menuOpen, GLuint casechecked);
bool handleGameActions();
void clicButton (SDL_Event e,int* playIsPush, float x, float y, int* menuOpen, char* mapName, GLuint casechecked );

#endif
