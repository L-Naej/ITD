#include "mapDrawer.h"
#include "graphics.h"

void drawWorld(World* world){
	if(world == NULL) return;
	int i = 0;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
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
	
	drawTowers(world->towersList);
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
	switch(monster->type){
		case BLUE_OCTOPUS : monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID;
		break;
		case ORANGE_OCTOPUS : monsterTexture = GAME_TEXTURES_ID.ORANGE_OCTOPUS_ID;
		break;	
		case GREEN_OCTOPUS : monsterTexture = GAME_TEXTURES_ID.GREEN_OCTOPUS_ID;
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
	glColor3ub(255,0,0);
	//Dessin de la vie
	float barSize = monster->life / ((float)monster->maxLife) * (MONSTER_WIDTH_PX / 1.2);
	float decalY = - (MONSTER_WIDTH_PX / 1.2 - barSize) / 2.0;
	glTranslatef(MONSTER_WIDTH_PX / 2.0 , decalY, 0.);
	glScalef(2.0, barSize, 1.);
	drawQuad();
	glPopMatrix();
}

void drawTowers(List* towersList){
	if(towersList == NULL) return;
	
	Tower* cur = NULL;
	GLuint textureId = -1;
	goToHeadList(towersList);
	while ( (cur = (Tower*) nextData(towersList)) != NULL){
		switch(cur->type){
		case GUN : textureId = GAME_TEXTURES_ID.GUN_TOWER_ID;
		break;
		case LASER : textureId = GAME_TEXTURES_ID.LASER_TOWER_ID;
		break;
		case ROCKET : textureId = GAME_TEXTURES_ID.ROCKET_TOWER_ID;
		break;
		case HYBRID : textureId = GAME_TEXTURES_ID.HYBRID_TOWER_ID;
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
	}	
}

//Pour l'instant taille des cercles etc fixée en dur
void drawPath(const Map* map){
	if(map == NULL) return;
	int radius = 10;
	
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


