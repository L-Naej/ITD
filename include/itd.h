#ifndef ITD_ITD_H
#define ITD_ITD_H
#include "list.h"
#include "world.h"
#include "interfaceDrawer.h"

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/*Longueur max d'un nom de fichier*/
static const unsigned int MAX_FILE_LENGTH = 30;

/**
 * Fonction principale du programme
 * ITD. Initialise le jeu et contient 
 * la boucle de jeu.
 */
int main(int argc, char* argv[]);

void cleanExit(World* world, Interface* interface);
#endif
