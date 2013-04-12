#ifndef ITD_ITD_H
#define ITD_ITD_H

/**
 * Défini les modes dans lequel le programme
 * peut être. Mode MENU le programme affiche le menu
 * de sélection de carte. 
 */
typedef enum{
	/**
	 * Mode MENU, le programme affiche le menu
 	 * de sélection de carte.
	 */
	MENU,
	/**
	 * Mode GAME, le programme affiche la carte
	 * du jeu et l'interface utilisateur de jeu.
	 */
	GAME
}Mode;

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/**
 * Initialise la fenêtre SDL du
 * programme ITD. Créé le contexte 
 * OpenGL.
 */
int initWindow();

///Fonctions interne pour la gestion de la fenêtre
static int setVideoMode();
static void reshape();

void initMenu();

/**
 * Fonction principale du programme
 * ITD. Initialise le jeu et contient 
 * la boucle de jeu.
 */
int main(int argc, char** argv);
#endif
