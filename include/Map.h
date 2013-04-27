#ifndef ITD_MAP_H
#define ITD_MAP_H
#include "list.h"
#include "tower.h"

typedef struct{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}Color3u;

typedef struct{
	int x;
	int y;
}PathNode;

typedef struct{
	char* name;
	int width;
	int height;
	
	Color3u pathColor;
	Color3u nodeColor;
	Color3u constructAreaColor;
	Color3u inAreaColor;
	Color3u outAreaColor;
	
	List* nodeList;
}Map;

/**
 * Retourne une structure Map initialisée
 * (tous les membres à NULL/0).
 */
Map initMap();

/**
 * charge les infos venant du .itd
 * dans la structure Map.
 */
int loadMap(Map* map, Tower* rocket, Tower* laser, Tower* mitraillette, Tower* hybrid);

/**
 * Retourne une structure Color3u initialisée
 * (tous les membres à NULL/0).
 */
Color3u initColor();

/**
 * Fonction de debug, affiche dans la
 * console les paramètres de la Map map.
 */
void dumpMap(Map map);

/**
 * Fonction de debug, affiche dans la
 * console les paramètres de la Color3u color.
 */
void dumpColor3u(Color3u color);
#endif
