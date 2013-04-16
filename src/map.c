#include <stdlib.h>
#include <stdio.h>
#include "Map.h"

Color3u initColor(){
	Color3u newColor;
	newColor.red = newColor.green = newColor.blue = 0;
	return newColor;
}

Map initMap(){
	Map newMap;
	newMap.name = NULL;
	newMap.width = 0;
	newMap.height = 0;

	newMap.pathColor = initColor();
	newMap.nodeColor = initColor();
	newMap.constructAreaColor = initColor();
	newMap.inAreaColor = initColor();
	newMap.outAreaColor = initColor();

	newMap.nodeList.head = NULL;
	newMap.nodeList.bottom = NULL;
	newMap.nodeList.cursor = NULL;
	newMap.nodeList.size = 0;
	newMap.nodeList.position = 0;
	newMap.nodeList.type = NODE;

	return newMap;
}

int chargeMap(Map* map, char* path_itd){	

	// On ouvre le fichier 
	FILE* file;
	
	file = fopen(path_itd,"r");
	if(file == NULL)
		return 0;
	else{
		//pour sauter deux caractères du .itd (@ITD 1). J'ai mis 5 mais je ne sais pas si on compte l'espace
		fseek(file,5,SEEK_SET);
		
		// nom de l'image
		(map->name) = (char*)malloc(sizeof(char));
		fscanf(file,"%s \n",&(map->name));

		//heu... l'énergie c'est quoi?
		//et width et height ne sont pas dans le .itd apparement

		fscanf(file,"%d %d %d\n",&(map->pathColor.red),&(map->pathColor.green),&(map->pathColor.blue));

		fscanf(file,"%d %d %d\n",&(map->nodeColor.red),&(map->nodeColor.green),&(map->nodeColor.blue));
		
		fscanf(file,"%d %d %d\n",&(map->constructAreaColor.red),&(map->constructAreaColor.green),&(map->constructAreaColor.blue));

		fscanf(file,"%d %d %d\n",&(map->inAreaColor.red),&(map->inAreaColor.green),&(map->inAreaColor.blue));

		fscanf(file,"%d %d %d\n",&(map->outAreaColor.red),&(map->outAreaColor.green),&(map->outAreaColor.blue));
		
		fscanf(file,"%d\n",&(map->nodeList.size));

		int coordx;
		int coory;
		fscanf(file,"%d %d\n",coordx,coordy);

// si j'ai bien compris on fait une liste de noeud pour le chemin. Donc il faudrait mettre 2 userdata dans chaque noeud non?

		List* listPath = createList(NODE,coordx,coordy); 

		int i=0;
		while (i<(map->nodeList.size)-1){
			fscanf(file,"%d %d\n",coordx,coordy);
			insertBottomCell(listPath,coordx,coordy);
		}
		
		map->nodeList.head = goToHeadCell(listPath);
		map->nodeList.bottom = goToBottomCell(listPath);

		

		// On vide  le buffer et on ferme le fichier
		fflush(file);
		fclose(file);
		return 1;
	}
}


void dumpColor3u(Color3u color){
	printf("(r:%d, g:%d, b:%d)", color.red, color.green, color.blue);
}

void dumpMap(Map map){
	printf("-------- DUMP MAP %s --------\n", map.name == NULL ? "" : map.name);
	printf("Width : %d\n", map.width);
	printf("Height : %d\n", map.height);
	printf("Path color : ");
	dumpColor3u(map.pathColor);
	printf("\nNode color : ");
	dumpColor3u(map.nodeColor);
	printf("\nConstructable area color : ");
	dumpColor3u(map.constructAreaColor);
	printf("\nIn Area color : ");
	dumpColor3u(map.inAreaColor);	
	printf("\nOut Area color : ");
	dumpColor3u(map.outAreaColor);

	printf("\nNode list (%d nodes)\n", map.nodeList.size);
	goToHeadList(&map.nodeList);

	PathNode* cur = NULL;
	while( (cur = nextData(&map.nodeList)) != NULL){
		printf("Node %d : x=%d y=%d\n", map.nodeList.position, cur->x, cur->y);
	}
}
