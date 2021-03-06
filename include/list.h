#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include "utils.h"



/**
 * Représente une cellule d'une liste,
 * donc reliée à deux autres cellules (ou
 * une seule si en tête ou queue de liste).
 */
typedef struct cell{
	struct cell* next;
	struct cell* previous;
	void* userData;
} Cell;

/**
 * Représente la liste en elle même. 
 * Un utilisateur de cette bibliothèque ne devrait
 * utiliser que cette structure.
 */
typedef struct list{
	Cell* head;
	Cell* cursor;/*Le curseur permet de naviguer dans la liste*/
	Cell* bottom;
	
	int size;
	int position;
} List;

/**
 * Structure rajoutée pour la sauvegarde de 
 * l'état de la liste. Retournée par la fonction
 * saveListState(List*), et fournie en argument
 * à restoreListState(ListState*), cette structure permet à 
 * une fonction de se déplacer au sein d'une liste et
 * de la repositionner à son état initial après coup.
 */
typedef struct list_state{
	struct list* savedList;
	struct cell* savedCell;
	int savedPosition;
} ListState;

/**
 * Créé une cellule dont le champs Cell->userData pointe 
 * sur userData.
 * Cette cellule n'est reliée à aucune autre.
 */
Cell* createCell(void* userData);

/**
 * Créé une liste de Cell vide.
 */ 
List* createEmptyList();
/**
 * Créé une liste de Cell ayant pour premier élément
 * une Cell contenant headUserData. Place le curseur sur 
 * la première donnée de la liste.
 * @param headUserData le premier élément de la liste créée.
 */
List* createList(void* headUserData);

/**
 * Renvoie le Cell courrant de la liste list
 * (le Cell sur lequel pointe le curseur).
 */
Cell* currentCell(List* list);

/**
 * Déplace le curseur de la liste sur le
 * prochain Cell et le renvoie.
 * Si le curseur était sur la dernère cellule,
 * renvoie NULL mais ne bouge pas le curseur.
 */
Cell* nextCell(List* list);

/**
 * Déplace le curseur sur le Cell précédent de la
 * liste et le renvoie.
 */
Cell* previousCell(List* list);

/**
 * Renvoie la data courrante de la liste list
 * (contenue dans le Cell sur lequel pointe le curseur).
 */
void* currentData(List* list);

/**
 * Déplace le curseur de la liste sur le
 * prochain Cell et renvoie le userData qu'il contient.
 */
void* nextData(List* list);

/**
 * Déplace le curseur sur le Cell précédent de la
 * liste et renvoie le userData qu'il contient.
 */
void* previousData(List* list);

/**
 * Compte et renvoie le nombre d'éléments que contient la liste.
 * Ne devrait pas être appelée par une fonctione extérieure de la librairie,
 * utiliser le champ size de la structure List à la place.
 * Renvoie 0 si list est égale à NULL
 * @param list la liste que l'on compte
 * Note: voir l'utilité de cette fonction avec le champs size...
 */
int listCountElem(List* list);

/**
 * Test si une liste est vide.
 * Renvoie true si list==NULL.
 */
bool isListEmpty(List* list);

/**
 * Insère un Cell en tête de liste contenant
 * userData.
 * Ne déplace pas le curseur.
 */
Cell* insertHeadCell(List* list, void* userData);

/**
 * Insère un Cell en fin de listecontenant
 * userData.
 * Ne déplace pas le curseur.
 */
Cell* insertBottomCell(List* list, void* userData);

/**
 * Insère le Cell lToInsert contenant
 * userData à la suite du curseur.
 * Ne déplace pas le curseur.
 */
Cell* insertAfterCell(List* list, void* userData);

/**
 * Insère le Cell lToInsert contenant
 * userData avant le curseur.
 * Ne déplace pas le curseur.
 */
Cell* insertBeforeCell(List* list, void* userData);

/**
 * Positionne le curseur en tête de liste (donc à NULL).
 * Attention! : un insertAfterCell() après un appel à cette fonction
 * positionnera le nouveau Cell en tête de liste (et non en seconde
 * position). insertBeforeCell() fera de même.
 */
void goToHeadList(List* list);

/**
 * Positionne le curseur en fin de liste.
 * Renvoie le pointeur vers le dernier élément de la liste.
 */
Cell* goToBottomCell(List* list);

/**
 * Se positionne à la cellule cell.
 * Si la cell ne fait pas partie de la liste,
 * ne bouge pas le curseur.
 * Renvoie la position de la cellule, -1 si elle
 * n'a pas été trouvée (cell n'existe pas).
 */
int goToCell(List* list, Cell* cell);

/**
 * Se positionne à la cellule contenant la donnée data.
 * Si la donnée ne fait pas partie de la liste,
 * ne bouge pas le curseur.
 * Renvoie la position de la cellule, -1 si elle
 * n'a pas été trouvée (userData n'existe pas).
 */
int goToData(List* list, void* userData);

/**
 * Se positionne à la cellule numéro
 * 'position'. Si la position est
 * plus grande que la taille, ne bouge pas le
 * curseur. Renvoie true si la position est valide,
 * false sinon.
 * Position valide : 0 < position <= list->size
 */
bool goToPosition(List* list, int position);

/**
 * Renvoie le Cell de la liste étant à la position
 * demandée (le premier Cell est à la position 1).
 * Positionne le curseur sur cette cellule.
 */
Cell* getCellByPosition(List* list, int position);


/**
 * Enlève la dernière cellule de la liste
 * et retourne le userData qu'elle contenait
 */
void* delBottomCell(List* list);

/**
 * Enlève la dernière cellule de la liste
 * et supprime de la mémoire le userData qu'elle
 * contenait.
 */
void freeBottomCell(List* list);

/**
 * Enlève le Cell de la liste étant à la position
 * demandée (le premier Cell est à la position 1).
 * Libère la mémoire allouée pour le Cell mais pas pour le userData.
 * Si le curseur était sur le Cell enlevé, il va sur le Cell 
 * ayant pris sa position dans la liste.
 * Renvoie le pointeur vers le userData enlevé de la liste.
 */
void* delCellByPosition(List* list, int position);

/**
 * Supprime le Cell de la liste étant à la position
 * demandée (le premier Cell est à la position 1).
 * Si le curseur était sur le Cell enlevé, il va sur le Cell 
 * ayant pris sa position dans la liste.
 * Libère la mémoire allouée, userData n'existe plus.
 */
void freeCellByPosition(List* list, int position);

/**
 * Enlève le Cell l de la liste et libère la mémoire prise par la
 * structure Cell.
 * Ne libère pas la mémoire allouée par le userData du Cell qui existe toujours.
 * Si le curseur était sur le Cell enlevé, il va sur le Cell 
 * ayant pris sa position dans la liste.
 * Renvoie le pointeur vers le userData du Cell enlevé de la liste.
 */
void* delCellInList(List* list, Cell* c);

/**
 * Supprime le Cell l de la liste et libère la mémoire prise par la
 * structure Cell.
 * Si le curseur était sur le Cell enlevé, il va sur le Cell 
 * ayant pris sa position dans la liste.
 * Libère la mémoire allouée pour le userData qui n'existe plus.
 */
void freeCellInList(List* list, Cell* c);

/**
 * Vide la mémoire prise par la Cell pointée par
 * cell. Ne libère pas la mémoire pointée par le userData.
 */
void freeCell(Cell* cell);

/*
 * Libère une liste de la zone mémoire
 * mais ne supprime pas les userData des cellules.
 */
void freeList(List* list);

/*
 * Libère une liste de la zone mémoire
 * ainsi que les userData contenus dans ses cellules.
 */
void freeListComplete(List* list);

/**
 * Voir struct Cursor définie en tête de ce
 * fichier.
 * Ne pas oublier de libérer la mémoire
 * allouée pour le ListState après utilisation.
 */
ListState* saveListState(List* list);

/**
 * Voir struct Cursor définie en tête de ce 
 * fichier.
 * Ne pas oublier de libérer la mémoire
 * allouée pour le ListState après utilisation.
 */
void restoreListState(ListState* state);

/**
 * Affiche une liste dans la console (debug, stderr)
 */
void dumpList(List* list);
#endif

