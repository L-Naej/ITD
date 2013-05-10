#ifndef ITD_INTERFACE_DRAWER
#define ITD_INTERFACE_DRAWER
#include <SDL/SDL_ttf.h>

void drawInterface();
void drawMenu();
void drawButton(void);
void drawCarre(void);

SDL_Surface* loadFont(TTF_Font*, char* str,char* rootPath, int taille);
void drawMenu(GLuint ,GLuint,GLuint ,GLuint , GLuint,GLuint,GLuint,GLuint,GLuint,GLuint,GLuint, GLuint,GLuint, int*,int*,int*, char*);
void drawMapMenu (GLuint carteMarine,GLuint carteSpatiale, GLuint casevide, GLuint casechecked, char* mapName);


#endif
