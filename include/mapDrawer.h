#ifndef ITD_MAP_DRAWER_H
#define ITD_MAP_DRAWER_H
#include "world.h"

//Temps d'affichage d'un missile en ms
static const Uint32 MISSILE_TIME_TO_LIVE_MS = 100; 
typedef struct{
	Point3D startPoint;
	Point3D endPoint;
	Uint32 startTime;
	Color3u color;
}Missile;

void drawWorld(World* world);

//Fonctions internes
void drawSky();
void drawMap(const Map* map);
void drawMonster(const Monster* monster);
void drawTowers(World* world, List* towersList);
void drawPath(const Map* map);
Missile* createMissile(Point3D startPoint, Point3D endPoint, Uint32 startTime);
#endif
