#ifndef INTERFACE_DRAWER
#define INTERFACE_DRAWER
#include <SDL/SDL_ttf.h>

void drawButton(void);
void drawCarre(void);
GLuint makeTexture (SDL_Surface*);
SDL_Surface* drawMapMenu(TTF_Font*);

#endif
