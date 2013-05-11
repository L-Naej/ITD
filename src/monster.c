#include "monster.h"
#include <math.h>
#include <stdio.h>
#include "geometry.h"

Monster createMonster(unsigned char wave, int nbMonstersCreated){

	MonsterType type;
	if(wave < 10) type = BLUE_OCTOPUS;
	else if (wave < 15){
		if(nbMonstersCreated < (wave - 9))
			type = ORANGE_OCTOPUS;
		else type = BLUE_OCTOPUS;
	}
	else{
		if(nbMonstersCreated < (wave -14))
			type = GREEN_OCTOPUS;
		else type = ORANGE_OCTOPUS;
	}

		
	switch(type){
		case BLUE_OCTOPUS : return createBlueOctopus(wave);
		break;
		case ORANGE_OCTOPUS : return createOrangeOctopus(wave);
		break;
		case GREEN_OCTOPUS : return createGreenOctopus(wave);
		break;
		default : return createBlueOctopus(wave);
		break;
	}
}

Monster createBlueOctopus(unsigned char wave){
	Monster monster;
	monster.type = BLUE_OCTOPUS;
	monster.life = 3*wave;
	monster.rocketResistance = wave;
	monster.laserResistance = wave;
	monster.gunResistance = 1;
	monster.hybridResistance = 1;
	monster.money = 5*wave;
	monster.speed = 2;
	
	monster.position = PointXYZ(-1,-1,0);
	monster.destination = PointXYZ(-1,-1,0);
	monster.direction = VectorXYZ(0.,0.,0.);
	monster.nbTurnsSinceLastMove = 0;
	
	
	return monster;
}

Monster createOrangeOctopus(unsigned char wave){
	Monster monster;
	monster.type = ORANGE_OCTOPUS;
	monster.life = 3*wave;
	monster.rocketResistance = wave;
	monster.laserResistance = wave;
	monster.gunResistance = 1;
	monster.hybridResistance = 1;
	monster.money = 5*wave;
	monster.speed = 2;
	
	monster.position = PointXYZ(-1,-1,0);
	monster.destination = PointXYZ(-1,-1,0);
	monster.nbTurnsSinceLastMove = 0;
		monster.direction = VectorXYZ(0.,0.,0.);
	
	return monster;
}

Monster createGreenOctopus(unsigned char wave){
	Monster monster;
	monster.type = GREEN_OCTOPUS;
	monster.life = 3*wave;
	monster.rocketResistance = wave;
	monster.laserResistance = wave;
	monster.gunResistance = 1;
	monster.hybridResistance = 1;
	monster.money = 5*wave;
	monster.speed = 2;
	
	monster.position = PointXYZ(-1,-1,0);
	monster.destination = PointXYZ(-1,-1,0);
	monster.nbTurnsSinceLastMove = 0;
		monster.direction = VectorXYZ(0.,0.,0.);
	
	return monster;
}

/* Explication de la détermination du pixel vers lequel le monstre doit aller :
 * On sait qu'à chaque tour un monstre avance au maximum d'un pixel.
 * Cette fonction a donc pour rôle de déplacer un monstre d'un pixel dans la bonne direction
 * (elle ne gère pas sa vitesse qui est gérée dans World.c).
 * On connait le point de départ du monstre qui est sa position actuelle, et son point
 * d'arrivée (qui est le prochain PathNode). A partir de là on connait sa direction. Le calcul de la direction à chaque déplacement
 * permet d'éviter la propagation des erreurs.
 * Il faut donc choisir s'il se déplace d'un pixel en X ou en Y.
 * Pour cela on regarde si la direction est plus proche de l'axe X ou de l'axe Y grâce au produit scalaire.
 * On compare les deux valeurs, et la plus grande détermine l'axe dans lequel va se déplacer le monstre.
 * Il faut regarder le signe de la valeur pour savoir si on va en -X ou X / -Y ou Y.
 * MAJ : Il a fallut rajouter une correction des erreurs de trajectoire car en effet sur un déplacement
 * il y a toujours une composante majoritaire et les monstres ne suivaient pas la ligne.
 */
void moveMonster(Monster* monster){
	if(monster == NULL) return;
	bool monsterMove = false;
	monsterMove = monster->nbTurnsSinceLastMove >= monster->speed;
	if(!monsterMove) return;
	
	float factorX, factorY;
	
	Vector3D direction = Normalize(Vector(monster->position, monster->destination));
	
	monster->realPosition = PointPlusVector(monster->realPosition, Normalize(monster->direction));
	
	
	factorX = DotProduct(direction, ITD_X_AXIS);
	factorY = DotProduct(direction, ITD_Y_AXIS);
	
	//Principe : on projette le vecteur direction sur axe des x et axe des y,
	//on regarde sur quel axe on se déplace le plus, et ensuite on déplace
	//le monstre d'un pixel sur cet axe.
	if(fabs(factorX) > fabs(factorY)){
		if(factorX > 0) monster->position.x++;
		else monster->position.x--;
	}
	else if(fabs(factorY) > fabs(factorX)){
		if(factorY > 0) monster->position.y++;
		else monster->position.y--;
	}
	else if((fabs(factorX)-fabs(factorY)) < 0.00001){
		if(factorY > 0) monster->position.y++;
		else monster->position.y--;	
		if(factorX > 0) monster->position.x++;
		else monster->position.x--;
	}
	//Correction des erreurs de trajectoire
	if(monster->realPosition.x - monster->position.x > 1) monster->position.x++;
	else if (monster->realPosition.x - monster->position.x < -1)monster->position.x--;
	if(monster->realPosition.y - monster->position.y > 1) monster->position.y++;
	else if(monster->realPosition.y - monster->position.y < -1) monster->position.y--; 
	monster->nbTurnsSinceLastMove = 0;



}
