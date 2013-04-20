#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Map.h"
#include "utils.h"

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
	
	return newMap;
}

int loadMap(Map* map){	

	// On ouvre le fichier 
	FILE* file;

	file = fopen("map/map1.itd","r");
	if(file == NULL)
		return 0;
	else{
		char versionMap [30];
		fscanf(file,"%c %c %c %c %c %c \n",versionMap);
		// la je sais pas du tout si c'est comme ça qu'il faut faire
		if (strcmp(versionMap,"@ITDSP1")!= 0 && strcmp(versionMap,"@ITDSP2")!=0 ){
			printf("Fichier incompatible");
			return;
		} 

		if (strcmp(versionMap,"@ITDSP1")==0){

			// nom de l'image
			(map->name) = (char*)malloc(sizeof(char)*30); //  on a dit que le nom de la carte ferait 30 caractères maxi
			fscanf(file,"%s \n",map->name); // par contre je sais pas comment faire vu qu'on connait pas la taille du nom. %s va surement pas marcher

			//rajouter width et height quand on aura le fichier .ppm
			int R,V,B;
			fscanf(file,"%d %d %d\n",&R,&V,&B);
			map->pathColor.red = float32toubyte8(R);
			map->pathColor.green = float32toubyte8(V);
			map->pathColor.blue = float32toubyte8(B);


			fscanf(file,"%d %d %d\n",&R,&V,&B);
			map->nodeColor.red = float32toubyte8(R);
			map->nodeColor.green = float32toubyte8(V);
			map->nodeColor.blue = float32toubyte8(B);

			fscanf(file,"%d %d %d\n",&R,&V,&B);
			map->constructAreaColor.red = float32toubyte8(R);
			map->constructAreaColor.green = float32toubyte8(V);
			map->constructAreaColor.blue = float32toubyte8(B);

			fscanf(file,"%d %d %d\n",&R,&V,&B);
			map->inAreaColor.red = float32toubyte8(R);
			map->inAreaColor.green = float32toubyte8(V);
			map->inAreaColor.blue = float32toubyte8(B);

			fscanf(file,"%d %d %d\n",&R,&V,&B);
			map->outAreaColor.red = float32toubyte8(R);
			map->outAreaColor.green = float32toubyte8(V);
			map->outAreaColor.blue = float32toubyte8(B);

			fscanf(file,"%d\n",&(map->nodeList.size));


			PathNode node1; 
			fscanf(file,"%d %d\n",&(node1.x),&(node1.y));
			

			map->nodeList = createList(node1); 

			int i=0;
			while (i<(map->nodeList->size)-1){
				PathNode node;
				fscanf(file,"%d %d\n",&(node.x),&(node.y));
				insertBottomCell(map->nodeList,node);
			}



			// On vide  le buffer et on ferme le fichier
			fflush(file);
			fclose(file);
			return 1;
		}
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
