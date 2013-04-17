#ifndef ITD_ITD_H
#define ITD_ITD_H


/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/* Unité de temps d'un pas de jeu = 10ms */
static const Uint32 TIMESTEP_MILLISECONDS = 10;

/*Longueur max d'un nom de fichier*/
static const unsigned int MAX_FILE_LENGTH = 30;

/**
 * Initialise la fenêtre SDL du
 * programme ITD. Créé le contexte 
 * OpenGL.
 */
int initWindow();

///Fonctions interne pour la gestion de la fenêtre
static int setVideoMode();
static void reshape();

/*
 * Initialise la liste des maps disponibles
 * et retourne une liste la contenant.
 */
List initMenu();

/**
 * Fonction principale du programme
 * ITD. Initialise le jeu et contient 
 * la boucle de jeu.
 */
int main(int argc, char* argv[]);
#endif
