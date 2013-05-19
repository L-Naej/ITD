#include "mapDrawer.h"
#include "graphics.h"
#include <math.h>

void drawWorld(World* world){
	if(world == NULL) return;
	int i = 0;
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLineWidth(1.0);
	
	//Important car les textures sont multipliées par la couleur courante
	glColor3ub(255,255,255);
	glPushMatrix();
	glTranslatef(world->cameraPosition.x, world->cameraPosition.y, world->cameraPosition.z);
	drawMap(&(world->map));
	
	drawPath(&(world->map));
	
	if( ! world->isBetweenWaves){
		for(i = 0; i < MONSTERS_PER_WAVE; ++i){
			if(world->monsters[i].life > 0)
				drawMonster(&(world->monsters[i]));
		}
	}
	
	drawTowers(world, world->towersList);
	glPopMatrix();
	
}

//TODO (rajouter le chemin etc)
void drawMap(const Map* map){
	glPushMatrix();
	glScalef(map->width,map->height,1.);
	drawTexturedQuad(GAME_TEXTURES_ID.MAP_ID);
	glPopMatrix();
}

void drawMonster(const Monster* monster){
	if(monster == NULL) return;
	GLuint monsterTexture = 0;
	Vector3D direction = Normalize(monster->direction);
	float factorX = DotProduct(direction, ITD_X_AXIS);
	float factorY = DotProduct(direction, ITD_Y_AXIS);
	float epsilon = 0.4;
	switch(monster->type){
		case BLUE_OCTOPUS :
			if (factorY <= -1. + epsilon && fabs(factorX) < epsilon){
				monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID;
			}
			else if (factorY > 1. - epsilon){
				monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID_DOS;
			}
			else if (factorX < -1. + epsilon && fabs(factorY) < epsilon){
				monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID_GAUCHE;
			}
			else if (fabs(factorX) >= 1.- epsilon && fabs(factorY) < epsilon){
				monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID_DROITE;
			}
			else if ( fabs(factorX + 0.5)<epsilon){
				monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID_DEMI_GAUCHE;
			}
			else if (-factorX-0.5<epsilon){
				monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID_DEMI_DROITE;
			}
			else monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID;
		break;
		case ORANGE_OCTOPUS : 			
			if (factorY <= -1. + epsilon && fabs(factorX) < epsilon){
				monsterTexture = GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID;
			}
			else if (factorY > 1. - epsilon){
				monsterTexture = GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID_DOS;
			}
			else if (factorX < -1. + epsilon && fabs(factorY) < epsilon){
				monsterTexture = GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID_GAUCHE;
			}
			else if (factorX >= 1.- epsilon && fabs(factorY) < epsilon){
				monsterTexture = GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID_DROITE;
			}
			else if (fabs(factorX+0.5)<epsilon){
				monsterTexture = GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID_DEMI_GAUCHE;
			}
			else if (-factorX-0.5<epsilon){
				monsterTexture = GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID_DEMI_DROITE;
			}
			else monsterTexture = GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID;
		break;
		case GREEN_OCTOPUS : 
			if (factorY <= -1. + epsilon && fabs(factorX) < epsilon){
				monsterTexture = GAME_TEXTURES_ID.GREEN_OCTOPUS_ID;
			}
			else if (factorY > 1. - epsilon){
				monsterTexture = GAME_TEXTURES_ID.GREEN_OCTOPUS_ID_DOS;
			}
			else if (factorX < -1. + epsilon && fabs(factorY) < epsilon){
				monsterTexture = GAME_TEXTURES_ID.GREEN_OCTOPUS_ID_GAUCHE;
			}
			else if (factorX >= 1.- epsilon && fabs(factorY) < epsilon){
				monsterTexture = GAME_TEXTURES_ID.GREEN_OCTOPUS_ID_DROITE;
			}
			else if (fabs(factorX+0.5)<epsilon){
				monsterTexture = GAME_TEXTURES_ID.GREEN_OCTOPUS_ID_DEMI_GAUCHE;
			}
			else if (-factorX-0.5<epsilon){
				monsterTexture = GAME_TEXTURES_ID.GREEN_OCTOPUS_ID_DEMI_DROITE;
			}
			else monsterTexture = GAME_TEXTURES_ID.GREEN_OCTOPUS_ID;
		break;
		default : return;
		break;
	}
	
	glPushMatrix();
	glColor3ub(255,255,255);
	
	glTranslatef(monster->position.x, monster->position.y, monster->position.z);
	glPushMatrix();
	glScalef(MONSTER_WIDTH_PX,MONSTER_WIDTH_PX,1.0);
	drawTexturedQuad(monsterTexture);
	glPopMatrix();
	
	//Dessin de la vie
	float fullBarSize = (MONSTER_WIDTH_PX / 1.2);
	float barSize = monster->life / ((float)monster->maxLife) * fullBarSize;
	float decalY = - (MONSTER_WIDTH_PX / 1.2 - barSize) / 2.0;
	glPushMatrix();
	glTranslatef(MONSTER_WIDTH_PX / 2.0 + 10 , 0., 0.);
	
	glScalef(4.0, fullBarSize, 1.);
	glColor3ub(255,0,0);
	drawQuad();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(MONSTER_WIDTH_PX / 2.0 + 10 , decalY, 0.);
	glColor3ub(0,255,0);
	glScalef(4.0, barSize, 1.);
	drawQuad();
	glPopMatrix();
	glPopMatrix();
}

List* lstMissiles = NULL;
void drawTowers(World* world, List* towersList){
	if(towersList == NULL) return;
	if(lstMissiles == NULL) lstMissiles = createEmptyList();
	
	Tower* cur = NULL;
	GLuint textureId = -1;
	goToHeadList(towersList);
	while ( (cur = (Tower*) nextData(towersList)) != NULL){
		Color3u missileColor;
		switch(cur->type){
		case GUN : textureId = GAME_TEXTURES_ID.GUN_TOWER_ID;
		missileColor.red = 160; missileColor.green = 160; missileColor.blue = 0;
		break;
		case LASER : textureId = GAME_TEXTURES_ID.LASER_TOWER_ID;
		missileColor.red = 255; missileColor.green = 0; missileColor.blue = 0;
		break;
		case ROCKET : textureId = GAME_TEXTURES_ID.ROCKET_TOWER_ID;
		missileColor.red = 0; missileColor.green = 0; missileColor.blue = 255;
		break;
		case HYBRID : textureId = GAME_TEXTURES_ID.HYBRID_TOWER_ID;
		missileColor.red = 0; missileColor.green = 255; missileColor.blue = 0;
		break;
		default : textureId = GAME_TEXTURES_ID.LASER_TOWER_ID;
		break;
		}
		glPushMatrix();
		glColor3ub(255,255,255);
		glTranslatef(cur->position.x, cur->position.y, cur->position.z);
		glScalef(TOWER_WIDTH_PX, TOWER_HEIGHT_PX, 1.0);
		drawTexturedQuad(textureId);
		glPopMatrix();
		
		//Enregistrement des nouveaux missiles
		Point3D* curTarget = NULL;
		goToHeadList(cur->whereIShoot);
		while( (curTarget = (Point3D*) nextData(cur->whereIShoot)) != NULL){
			Missile* newMissile = createMissile(cur->position, *curTarget, SDL_GetTicks());
			newMissile->color = missileColor;
			insertBottomCell(lstMissiles, newMissile);
		}
	}	
	//Dessin des missiles en cours
	goToHeadList(lstMissiles);
	Missile* curMissile = NULL;
	while( (curMissile = (Missile*)nextData(lstMissiles)) != NULL){
		//Si le temps de vie du missile est dépassé on le détruit
		if((SDL_GetTicks() - curMissile->startTime) > MISSILE_TIME_TO_LIVE_MS || world->isBetweenWaves){
			freeCellByPosition(lstMissiles, lstMissiles->position);
		}
		else{
			glPushMatrix();
			glLineWidth(1.4);
			glColor3ub(curMissile->color.red,curMissile->color.green,curMissile->color.blue);
			glBegin(GL_LINES);
			glVertex2f(curMissile->startPoint.x, curMissile->startPoint.y);
			glVertex2f(curMissile->endPoint.x, curMissile->endPoint.y);
			glEnd();
			glPopMatrix();
		}
	}
}

//Pour l'instant taille des cercles etc fixée en dur
void drawPath(const Map* map){
	if(map == NULL) return;
	int radius = 10;
	glLineWidth(1.0);
	//Dessin des cercles, en prenant en compte les bonnes couleurs
	Point3D startNode = getStartPoint(map);
	Point3D endNode = getEndPoint(map);
	
	Point3D curNode = nextNode(map->pathNodeList, startNode);
	
	//Dessin du chemin
	glPushMatrix();
	glColor3ub(map->pathColor.red,map->pathColor.green,map->pathColor.blue);
	glBegin(GL_LINE_STRIP);
	glVertex2f(startNode.x, startNode.y);
	do{
		glVertex2f(curNode.x,curNode.y);
		curNode = nextNode(map->pathNodeList, curNode);
	}while(! arePointsEquals(curNode, endNode));	
	glVertex2f(endNode.x, endNode.y);
	glEnd();
	glPopMatrix();

	//Dessin du point de départ 
	glPushMatrix();
	glColor3ub(map->inAreaColor.red,map->inAreaColor.green,map->inAreaColor.blue);
	glTranslatef(startNode.x,startNode.y,startNode.z);
	glScalef(radius, radius, 1.);
	drawCircle(1);
	glPopMatrix();
	
	//Dessin du point d'arrivée
	glPushMatrix();
	glColor3ub(map->outAreaColor.red,map->outAreaColor.green,map->outAreaColor.blue);
	glTranslatef(endNode.x,endNode.y,endNode.z);
	glScalef(radius, radius, 1.);
	drawCircle(1);
	glPopMatrix();
	
	//Dessin des points intermédiaires
	curNode = nextNode(map->pathNodeList, startNode);
	glPushMatrix();	
	glColor3ub(map->pathColor.red,map->pathColor.green,map->pathColor.blue);
	do{
		
		glPushMatrix();
		glTranslatef(curNode.x, curNode.y, curNode.z);
		glScalef(radius, radius, 1.);
		drawCircle(1);
		glPopMatrix();
		curNode = nextNode(map->pathNodeList, curNode);
	}while(! arePointsEquals(curNode, endNode));
	

	glPopMatrix();
}

Missile* createMissile(Point3D startPoint, Point3D endPoint, Uint32 startTime){
	Missile* missile = (Missile*) malloc(sizeof(Missile));
	if(missile == NULL) {
		fprintf(stderr, "Erreur fatale : plus de mémoire disponible\n");
		return NULL;
	}
	missile->startPoint = startPoint;
	missile->endPoint = endPoint;
	missile->startTime = startTime;
	
	return missile;
}










