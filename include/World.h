#ifndef ITD_WORLD_H
#define ITD_WORLD_H
#include "Map.h"
#include "Tower.h"
#include "Monster.h"
#include "Utils.h"
#include "list.h"

static const unsigned char MONSTERS_PER_WAVE = 10;

/* Unité de temps d'un pas de jeu = 10ms */
static const Uint32 TIMESTEP_MILLISECONDS = 10;

/**
 * Structure représentant l'état du "monde",
 * c'est-à-dire de la partie en cours.
 * Choix de conception non définitifs...
 */
typedef struct{
	Uint32 worldTime;///Temps actuel dans le tour de jeu
	Map loadedMap;///Map chargée
	unsigned char currentMonstersWave;///Vague de monstres en cours (sur 20)
	Monster currentMonsters[MONSTERS_PER_WAVE];///Tableau des monstres actuellement sur la map.
	List currentTowersList;///Liste des tours actuellement sur la map.
}World;

/**
 * Variable globale contenant l'état du monde actuel.
 */
World currentWorld;

/**
 * Initialise un monde à partir du fichier .itd
 * dont le chemin est passé en paramètre.
 */
void initWorld(const char* pathToItdFile);

/**
 * Indique si la zone de la carte correspondant
 * au carré passé en paramètre est constructible.
 * x1,x2,y1,y2 correspondent aux coordonnées de deux
 * points opposés du carré.
 */
bool isAreaConstructible(int x1, int y1, int x2, int y2);

/**
 * Fait avancer le monde d'un pas de temps.
 * Appelée à chaque nouveau tour de boucle de 
 * jeu. Renvoie si le jeu est terminé ou non.
 */
bool worldNewStep(World* world);

/**
 * Fait tirer toutes les tours capable
 * de toucher un monstre au moment où
 * cette fonction est appelée.
 * (appelée à chaque pas de temps).
 */
void towersShoot(List* towerList);

/**
 * Fait bouger tous les monstres à chaque pas de temps. 
 * Sachant qu'un monstre se déplace au maximum d'un pixel
 * par pas de temps, se contentera souvent de simplement
 * mettre à jour la durée depuis laquel le monstre a
 * bougé depuis la dernière fois.
 */
void monstersMove(Monster* monsters);

#endif
