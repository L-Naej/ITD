#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Map.h"
#include "utils.h"
#define MAX_LENGHT 30

Color3u initColor(void){
	Color3u newColor;
	newColor.red = newColor.green = newColor.blue = 0;
	return newColor;
}

Map initMap(void){
	Map newMap;
	newMap.name = NULL;
	newMap.width = 0;
	newMap.height = 0;

	newMap.pathColor = initColor();
	newMap.nodeColor = initColor();
	newMap.constructAreaColor = initColor();
	newMap.inAreaColor = initColor();
	newMap.outAreaColor = initColor();

newMap.nodeList = createEmptyList();
	newMap.nodeList->head = NULL;
	newMap.nodeList->bottom = NULL;
	newMap.nodeList->cursor = NULL;
	newMap.nodeList->size = 0;
	newMap.nodeList->position = 0;


	return newMap;
}

int loadITD1 (Map* map, FILE* file){
	/* nom de l'image*/
	fseek (file,6,SEEK_CUR); 
	(map->name) = (char*)malloc(sizeof(char)*MAX_LENGHT);
 	/*  on a dit que le nom de la carte ferait 30 caractères maxi*/
	fscanf(file,"%s \n",map->name);


	/*rajouter width et height quand on aura le fichier .ppm*/

	int R,V,B;
			
	fseek (file,7,SEEK_CUR); 
	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->pathColor.red =(unsigned char)R;
	map->pathColor.green = (unsigned char)V;
	map->pathColor.blue =(unsigned char)B;
	if (testItdValid(map->pathColor.red,map->pathColor.green,map->pathColor.blue)!= 1){
		return 0;
	}


	fseek (file,6,SEEK_CUR); 
	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->nodeColor.red = (unsigned char)R;
	map->nodeColor.green = (unsigned char)V;
	map->nodeColor.blue = (unsigned char)B;
	if (testItdValid(map->nodeColor.red,map->nodeColor.green,map->nodeColor.blue)!= 1){
		return 0;
	}


	fseek (file,10,SEEK_CUR); 
	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->constructAreaColor.red = (unsigned char)R;
	map->constructAreaColor.green = (unsigned char)V;
	map->constructAreaColor.blue = (unsigned char)B;
	if (testItdValid(map->constructAreaColor.red,map->constructAreaColor.green,map->constructAreaColor.blue)!= 1){
		return 0;
	}


	fseek (file,3,SEEK_CUR); 
	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->inAreaColor.red = (unsigned char)R;
	map->inAreaColor.green = (unsigned char)V;
	map->inAreaColor.blue = (unsigned char)B;
	if (testItdValid(map->inAreaColor.red,map->inAreaColor.green,map->inAreaColor.blue)!= 1){
		return 0;
	}


	fseek (file,4,SEEK_CUR); 
	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->outAreaColor.red = (unsigned char)R;
	map->outAreaColor.green = (unsigned char)V;
	map->outAreaColor.blue = (unsigned char)B;
	if (testItdValid(map->outAreaColor.red,map->outAreaColor.green,map->outAreaColor.blue)!= 1){
		return 0;
	}


	fscanf(file,"%d\n",&(map->nodeList->size));



	PathNode* node1 = (PathNode*)malloc (sizeof(PathNode)); 
	fscanf(file,"%d %d\n",&(node1->x),&(node1->y));

	
	int size = map->nodeList->size;
	map->nodeList = createList((void*)node1); 

	int j=0;


	while (j<size-1){

		PathNode* node = (PathNode*)malloc (sizeof(PathNode));		
		fscanf(file,"%d %d\n",&(node->x),&(node->y));
		insertBottomCell(map->nodeList,(void*)node);
		j++;
	}

	int nbNode = listCountElem(map->nodeList);
	if (nbNode != map->nodeList->size){
		return 0;
	}
	return 1;
} 
int loadMap(Map* map){	

	/* On ouvre le fichier */
	FILE* file;

	file = fopen("map/map1.itd","r");
	if(file == NULL){
		return 0;
	}
	else{
		char versionMap [MAX_LENGHT];
		int k;
		for (k=0; k<MAX_LENGHT;k++){
			versionMap[k]= 0;
		}
		int i;
		for (i=0; i< 6; i++){
			fscanf(file,"%c",versionMap+i);
			printf("version : %s\n",versionMap+i);
		}
		/*la je sais pas du tout si c'est comme ça qu'il faut faire*/
		if (strcmp(versionMap,"@ITD 1")!= 0 && strcmp(versionMap,"@ITD 2")!=0 ){
			printf("Fichier incompatible");
			return 0;
		}
		
		 

		if (strcmp(versionMap,"@ITD 1")==0){
			if (loadITD1(map,file)==1){
				/* On vide  le buffer et on ferme le fichier*/
				fflush(file);
				fclose(file);
				printf("carte chargée");
				return 1;
			}
		}
		
		if (strcmp(versionMap,"@ITD 2")==0){	
			if (loadITD1(map,file)=!1){	
				return 0;
			}
				fseek (file,7,SEEK_CUR); 
				fscanf(file,"%d\n",&(tower->nodeList->size));
		}

	}
}

void dumpColor3u(Color3u color){
	printf("(r:%d, g:%d, b:%d)", color.red, color.green, color.blue);
}

void dumpMap(Map map){
	printf ("coucou\n");
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
	
	printf("\nNode list (%d nodes)\n", map.nodeList->size);
	goToHeadList(map.nodeList);
	
	PathNode* cur = NULL;
	while( (cur = nextData(map.nodeList)) != NULL){
		printf("Node %d : x=%d y=%d\n", map.nodeList->position, cur->x, cur->y);
	}
}
