#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"

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

	newMap.pathNodeList = createEmptyList();
	if(newMap.pathNodeList == NULL){
		perror("Erreur fatale : Impossible d'allouer la mémoire pour le chemin des monstres.\n");
		exit(-1);
	}
	newMap.image = NULL;

	return newMap;
}


Point3D nextNode(List* pathNodeList, Point3D currentNode){
	if(pathNodeList == NULL){
		fprintf(stderr, "Erreur fatale : La liste de noeuds de chemin passée en paramètre de nextNode est NULL.\n");
		exit(-1);
	}
	
	Point3D* tmpNode = NULL;
	bool pointIsNode = false;
	
	goToHeadList(pathNodeList);
	//On cherche le node courant
	/*Très important de tester pointIsNode en premier sinon on risque de se décaler encore une fois
	dans la liste alors qu'il ne faut pas */
	while( !pointIsNode && (tmpNode = (Point3D*) nextData(pathNodeList) ) != NULL){
		pointIsNode = arePointsEquals(currentNode, *tmpNode);
	}
	
	if(tmpNode == NULL){
		fprintf(stderr, "Erreur fatale, le point passé en paramètre de nextNode n'est pas un noeud valide.\n");
		exit(-1);
	}
	
	//Puis on retourne celui qui le suit.
	//La liste gère toute seule si elle est en fin de liste et ne "dépasse" pas (reste sur le dernier node)
	tmpNode = (Point3D*) nextData(pathNodeList);
	//Si on est sur le dernier point on le retourne lui même
	if(tmpNode == NULL) return currentNode;

	return *tmpNode;
}

Point3D getStartPoint(const Map* map){
	if(map == NULL) return PointXYZ(-1,-1,-1);
	
	goToHeadList(map->pathNodeList);
	Point3D* tmpPtr = (Point3D*) nextData(map->pathNodeList);
	if(tmpPtr == NULL){
		printf("startPoint NULL\n");
		exit(-1);
	}
	Point3D startPoint = *tmpPtr;
	
	return startPoint;
}

Point3D getEndPoint(const Map* map){
	if(map == NULL) return PointXYZ(-1,-1,-1);
	
	goToBottomCell(map->pathNodeList);
	Point3D* tmpPtr = (Point3D*) currentData(map->pathNodeList);
	if(tmpPtr == NULL){
		printf("endPoint NULL\n");
		exit(-1);
	}
	Point3D endPoint = *tmpPtr;
	
	return endPoint;
}

int testItdValid(unsigned char R,unsigned char V,unsigned char B){
	if ((R>=0 && R<= 255) && (V>=0 && V<= 255) && (B>=0 && B<= 255)){
		return 1;
	}
	else{
		return 0;
	}

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
		printf("ligne de couleur de chemin erronée \n");
		return 0;
	}


	fseek (file,6,SEEK_CUR); 
	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->nodeColor.red = (unsigned char)R;
	map->nodeColor.green = (unsigned char)V;
	map->nodeColor.blue = (unsigned char)B;
	if (testItdValid(map->nodeColor.red,map->nodeColor.green,map->nodeColor.blue)!= 1){
		printf("ligne de couleur de noeud erronée \n");
		return 0;
	}


	fseek (file,10,SEEK_CUR); 
	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->constructAreaColor.red = (unsigned char)R;
	map->constructAreaColor.green = (unsigned char)V;
	map->constructAreaColor.blue = (unsigned char)B;
	if (testItdValid(map->constructAreaColor.red,map->constructAreaColor.green,map->constructAreaColor.blue)!= 1){
		printf("ligne de couleur de zone constructible erronée \n");
		return 0;
	}


	fseek (file,3,SEEK_CUR); 
	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->inAreaColor.red = (unsigned char)R;
	map->inAreaColor.green = (unsigned char)V;
	map->inAreaColor.blue = (unsigned char)B;
	if (testItdValid(map->inAreaColor.red,map->inAreaColor.green,map->inAreaColor.blue)!= 1){
		printf("ligne de couleur d'entrée erronée \n");
		return 0;
	}


	fseek (file,4,SEEK_CUR); 
	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->outAreaColor.red = (unsigned char)R;
	map->outAreaColor.green = (unsigned char)V;
	map->outAreaColor.blue = (unsigned char)B;
	if (testItdValid(map->outAreaColor.red,map->outAreaColor.green,map->outAreaColor.blue)!= 1){
		printf("ligne de couleur de sortie erronée \n");
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
		if ((node->x)==0 && (node->y)==0){
			printf("nombre de coordonnée de noeuds incorrect - error 1- \n");
			return 0;
		}
		insertBottomCell(map->nodeList,(void*)node);
		j++;
	}

	/*int nbNode = listCountElem(map->nodeList);
	if (nbNode != map->nodeList->size){
		printf("nombre de coordonnée de noeuds incorrect - error 2- \n");
		return 0;
	}*/

	return 1;
} 
int loadMap(Map* map, Tower* rocket, Tower* laser, Tower* mitraillette, Tower* hybrid){	

	/* On ouvre le fichier */
	FILE* file;

	file = fopen("map/map2.itd","r");
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
			if (loadITD1(map,file)!=1){	
				return 0;
			}
				fseek (file,7,SEEK_CUR); 
				fscanf(file,"%d\n",&(rocket->power));
				fseek (file,6,SEEK_CUR); 
				fscanf(file,"%d\n",&(rocket->rate));
				fseek (file,7,SEEK_CUR); 
				fscanf(file,"%d\n",&(rocket->range));
				fseek (file,6,SEEK_CUR); 
				fscanf(file,"%d\n",&(rocket->cost));
	

				fseek (file,7,SEEK_CUR); 	
				fscanf(file,"%d\n",&(laser->power));
				fseek (file,6,SEEK_CUR); 
				fscanf(file,"%d\n",&(laser->rate));
				fseek (file,7,SEEK_CUR);
				fscanf(file,"%d\n",&(laser->range));
				fseek (file,6,SEEK_CUR); 
				fscanf(file,"%d\n",&(laser->cost));
		
				fseek (file,7,SEEK_CUR); 
				fscanf(file,"%d\n",&(mitraillette->power));
				fseek (file,6,SEEK_CUR); 
				fscanf(file,"%d\n",&(mitraillette->rate));
				fseek (file,7,SEEK_CUR); 
				fscanf(file,"%d\n",&(mitraillette->range));
				fseek (file,6,SEEK_CUR); 
				fscanf(file,"%d\n",&(mitraillette->cost));

				fseek (file,7,SEEK_CUR); 
				fscanf(file,"%d\n",&(hybrid->power));
				fseek (file,6,SEEK_CUR); 
				fscanf(file,"%d\n",&(hybrid->rate));
				fseek (file,7,SEEK_CUR); 
				fscanf(file,"%d\n",&(hybrid->range));
				fseek (file,6,SEEK_CUR); 
				fscanf(file,"%d\n",&(hybrid->cost));
			
		printf("  ROCKET power : %d, rate : %d, range : %d, cost : %d \n", rocket->power, rocket->rate, rocket->range, rocket->cost);
		printf("  LASER power : %d, rate : %d, range : %d, cost : %d \n", laser->power, laser->rate, laser->range, laser->cost);
		printf("  MITRAILLETTE power : %d, rate : %d, range : %d, cost : %d \n", mitraillette->power, mitraillette->rate, mitraillette->range, mitraillette->cost);
		printf("  HYBRID power : %d, rate : %d, range : %d, cost : %d \n", hybrid->power, hybrid->rate, hybrid->range, hybrid->cost);


		fflush(file);
		fclose(file);
		printf("carte chargée");
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
	
	printf("\nNode list (%d nodes)\n", map.pathNodeList->size);
	goToHeadList(map.pathNodeList);
	
	Point3D* cur = NULL;
	while( (cur = nextData(map.pathNodeList)) != NULL){
		printf("Node %d : x=%f y=%f\n", map.pathNodeList->position, cur->x, cur->y);
	}
}
