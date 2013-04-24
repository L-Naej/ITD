#include "monster.h"
#include <math.h>
#include <stdio.h>
#include "geometry.h"

Monster createMonster(unsigned char wave){
//TODO	
	Monster monster;
	
	monster.life = 3;
	monster.strenght = 1;
	monster.money = 5;
	monster.speed = 2;
	
	monster.position = PointXYZ(-1,-1,0);
	monster.destination = PointXYZ(-1,-1,0);
	monster.nbTurnsSinceLastMove = 0;

	return monster;
}

/* Explication de la détermination du pixel vers lequel le monstre doit aller :
 * On sait qu'à chaque tour un monstre avance au maximum d'un pixel.
 * Cette fonction a donc pour rôle de déplacer un monstre d'un pixel dans la bonne direction
 * (elle ne gère pas sa vitesse qui est gérée dans World.c).
 * On connait le point de départ du monstre qui est sa position actuelle, et son point
 * d'arrivée (qui est le prochaine PathNode). A partir de là on connait sa direction. Le calcul de la direction à chaque déplacement
 * permet d'éviter la propagation des erreurs.
 * Il faut donc choisir s'il se déplace d'un pixel en X ou en Y.
 * Pour cela on regarde dans l'espace réel de combien il se déplace en X et en Y selon
 * sa direction grâce à Pythagore (calcul de cos(alpha) pour trouver x, calcul de la pente
 * puis calcul de y=pente*x).
 * On compare les deux valeurs, et la plus grande détermine l'axe dans lequel va se déplacer le monstre.
 * Il faut regarder le signe de la valeur pour savoir si on va en -X ou X / -Y ou Y.
 */
void moveMonster(Monster* monster){
	if(monster == NULL) return;
	
	float moveX, moveY, cosAlpha, slope;//slope = pente
	
	Vector3D direction = Normalize(Vector(monster->position, monster->destination));
	
	cosAlpha = DotProduct(direction, ITD_X_AXIS);
	
	//Principe : on projette le vecteur direction sur axe des x et axe des y,
	//on regarde sur quel axe on se déplace le plus, et ensuite on déplace
	//le monstre d'un pixel sur cet axe.
	
	//Si direction parallèle à Y_AXIS (soit cosAlpha == 0), on regarde dans quel sens on va
	if(fabs(cosAlpha - 0.0) < 0.00001) {
		moveY = monster->destination.y - monster->position.y;
		moveX = 0.0;
	}
	else{
		slope = (monster->position.y - monster->destination.y) / (monster->position.x - monster->destination.x); 
		moveX =  cosAlpha;
		moveY = slope*moveX;
	}

	
	if(fabs(moveX) > fabs(moveY)){
		if(moveX > 0) monster->position.x++;
		else monster->position.x--;
	}
	else{
		if(moveY > 0) monster->position.y++;
		else monster->position.y--;
	}
}
