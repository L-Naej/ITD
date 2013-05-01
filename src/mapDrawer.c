#include "mapDrawer.h"
#include "graphics.h"

void drawWorld(const World* world){
	if(world == NULL) return;
	int i = 0;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	drawMap();
	
	if( ! world->isBetweenWaves){
		for(i = 0; i < MONSTERS_PER_WAVE; ++i){
			drawMonster(&(world->monsters[i]));
		}
	}
}

//TODO (rajouter le chemin etc)
void drawMap(){
	
	glPushMatrix();
	glLoadIdentity();
	
	//La map prend toute la fenêtre
	glScalef(WINDOW_WIDTH,WINDOW_HEIGHT,1.);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GAME_TEXTURES_ID.MAP_ID);
	glBegin(GL_QUADS);
	
	//Bottom left
	glTexCoord2f(0.0,1.0);
	glVertex2f(-0.5,-0.5);
	//Bottom right
	glTexCoord2f(1.0,1.0);
	glVertex2f(0.5,-0.5);

	//Top right
	glTexCoord2f(1.0,0.0);
	glVertex2f(0.5,0.5);
	//Top left
	glTexCoord2f(0.0,0.0);
	glVertex2f(-0.5, 0.5);

	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawMonster(const Monster* monster){
	if(monster == NULL) return;
	GLuint monsterTexture = 0;
	switch(monster->type){
		case BLUE_OCTOPUS : monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID;
		break;
		case ORANGE_OCTOPUS : monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID;
		break;	
		case GREEN_OCTOPUS : monsterTexture = GAME_TEXTURES_ID.BLUE_OCTOPUS_ID;
		break;	
		default : return;
		break;
	}
	Point3D oglPosition = sdlToOpenGL(monster->position);	
	
	glLoadIdentity();

	glTranslatef(oglPosition.x, oglPosition.y, 0.0);
	glScalef(MONSTER_WIDTH_PX,MONSTER_WIDTH_PX,1.0);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, monsterTexture);
	glBegin(GL_QUADS);
	
	//Bottom left
	glTexCoord2f(0.0,1.0);
	glVertex2f(-0.5,-0.5);
	//Bottom right
	glTexCoord2f(1.0,1.0);
	glVertex2f(0.5,-0.5);

	//Top right
	glTexCoord2f(1.0,0.0);
	glVertex2f(0.5,0.5);
	//Top left
	glTexCoord2f(0.0,0.0);
	glVertex2f(-0.5, 0.5);

	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
}

