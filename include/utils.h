#ifndef ITD_UTILS
#define ITD_UTILS
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

typedef enum{false=0, true=1}bool;

unsigned char int32toubyte8(int);

//Adapté depuis http://www.gamedev.net/page/resources/_/technical/game-programming/sdl--fonts-part-2-printstrings-r1960
SDL_Surface* PrintStringsOnSurface( TTF_Font *fnt, char* strings [], int nbStrings, SDL_Color clrFg );
#endif
