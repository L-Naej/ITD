#ifndef ITD_INTERFACE_DRAWER
#define ITD_INTERFACE_DRAWER
#include <SDL/SDL_ttf.h>

void drawInterface();
void drawMenu();
void drawButton(void);
void drawCarre(void);

SDL_Surface* drawMapMenu(TTF_Font*);

#endif
