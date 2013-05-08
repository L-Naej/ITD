#ifndef ITD_MAP_DRAWER_H
#define ITD_MAP_DRAWER_H
#include "World.h"
void drawWorld(const World* world);

//Fonctions internes
void drawMap(const Map* map);
void drawMonster(const Monster* monster);
void drawTowers(List* towersList);
#endif
