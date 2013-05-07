#ifndef ITD_UTILS
#define ITD_UTILS
#include <stdlib.h>

typedef enum{false=0, true=1}bool;

unsigned char int32toubyte8(int);

/**
 * Fonction utilitaire employée pour vérifier 
 * qu'une allocation mémoire s'est bien passée.
 * Cette fonction quitte le programme avec message d'erreur
 * si pointer vaut NULL. A utiliser après un malloc.
 */
 //TODO
void checkVitalAllocatio (void* pointer);
#endif
