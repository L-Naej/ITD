#ifndef ITD_MAP_H
#define ITD_MAP_H
#include <SDL/SDL.h>
#include "list.h"
#include "point3D.h"
#include "utils.h"

/** Note : les PathNodes sont implémentés par des Point3D **/

typedef struct{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}Color3u;

typedef struct{
	char* name;
	int width;
	int height;
	
	Color3u pathColor;
	Color3u nodeColor;
	Color3u constructAreaColor;
	Color3u inAreaColor;
	Color3u outAreaColor;
	
	Point3D startPoint;
	Point3D endPoint;
	List* pathNodeList;///Liste de Point3D
	
	SDL_Surface* pixelArray;
}Map;

/**
 * Retourne une structure Map initialisée
 * (tous les membres à NULL/0).
 */
Map initMap();

/**
 * charge les infos venant du .itd
 * dans la structure Map.
 * Retourne vrai si le chargement a réussi, faux sinon.
 */
bool loadMap(Map*, const char* pathToItdFile);

/**
 * Retourne une structure Color3u initialisée
 * (tous les membres à NULL/0).
 */
Color3u initColor();

/**
 * Retourne le prochain Node devant être atteint
 * à partir du Node passé en paramètre et de la liste de Node (le chemin
 * à suivre).
 * /!\ Si le point passé en paramètre ne correspond pas à un Node, quitte le programme
 * avec un message d'erreur.
 * /!\ Si la List passée en paramètre est NULL, quitte le programme avec message d'erreur.
 */
Point3D nextNode(List* pathNodeList, Point3D currentNode);

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
