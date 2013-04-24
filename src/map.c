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
	newMap.pixelArray = NULL;

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

bool loadMap(Map* map, const char* pathToItdFile){	
	if(pathToItdFile == NULL || map == NULL || map->pathNodeList == NULL)
		return false;
	
	/* On ouvre le fichier */
	FILE* file;

	file = fopen(pathToItdFile,"r");
	if(file == NULL){
		return false;
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
			return false;
		} 

		if (strcmp(versionMap,"@ITD 1")==0){

			/* nom de l'image*/
			(map->name) = (char*)malloc(sizeof(char)*MAX_LENGHT);
			if(map->name == NULL){
				perror("Erreur fatale : impossible d'allouer l'espace mémoire nécessaire dans loadMap\n");
				exit(-1);
			}
 /*  on a dit que le nom de la carte ferait 30 caractères maxi*/
			fscanf(file,"%s \n",map->name);
			printf("nom image : %s\n",map->name); /* par contre je sais pas comment faire vu qu'on connait pas la taille du nom. %s va surement pas marcher*/

			/*rajouter width et height quand on aura le fichier .ppm*/
			int R,V,B;
			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->pathColor.red =(unsigned char)R;
			map->pathColor.green = (unsigned char)V;
			map->pathColor.blue =(unsigned char)B;
			printf("couleur des chemins : %d %d %d\n",map->pathColor.red,map->pathColor.green,map->pathColor.blue);


			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->nodeColor.red = (unsigned char)R;
			map->nodeColor.green = (unsigned char)V;
			map->nodeColor.blue = (unsigned char)B;
			printf("couleur des noeuds : %d %d %d\n",map->nodeColor.red,map->nodeColor.green,map->nodeColor.blue);

			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->constructAreaColor.red = (unsigned char)R;
			map->constructAreaColor.green = (unsigned char)V;
			map->constructAreaColor.blue = (unsigned char)B;
			printf("couleur des zones constructibles : %d %d %d\n",map->constructAreaColor.red,map->constructAreaColor.green,map->constructAreaColor.blue);

			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->inAreaColor.red = (unsigned char)R;
			map->inAreaColor.green = (unsigned char)V;
			map->inAreaColor.blue = (unsigned char)B;
			printf("couleur des zones d'entrée : %d %d %d\n",map->inAreaColor.red,map->inAreaColor.green,map->inAreaColor.blue);


			fscanf(file,"%d %d %d\n",&R,&V,&B);
			printf("%d\n",R);
			printf("%d\n",V);
			printf("%d\n",B);
			map->outAreaColor.red = (unsigned char)R;
			map->outAreaColor.green = (unsigned char)V;
			map->outAreaColor.blue = (unsigned char)B;
			printf("couleur des zones de sortie : %d %d %d\n",map->outAreaColor.red,map->outAreaColor.green,map->outAreaColor.blue);
			
			int size;
			fscanf(file,"%d\n",&size);
			printf("nombre de noeuds : %d\n",size);

			
			/*
			Point3D* node1 = (Point3D*)malloc (sizeof(Point3D));
			if(node1 == NULL){
				fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire pour le chemin des monstres.\n");
				exit(-1);
			}
			
			fscanf(file,"%d %d\n",&(node1->x),&(node1->y));*/
			/*printf(" noeud : %d %d\n",node1->x,node1->y);*/
			
			/*printf("taille = %d",size);*/
			

			int j=0;

			while (j<size){
				Point3D* node = (Point3D*)malloc (sizeof(Point3D));
				if(node == NULL){
					fprintf(stderr, "Erreur fatale : impossible d'allouer la mémoire pour le chemin des monstres.\n");
					exit(-1);
				}		
				fscanf(file,"%f %f\n",&(node->x),&(node->y));
				node->z = 0;
			/*printf(" noeud : %d %d\n",node->x,node->y);*/
				insertBottomCell(map->pathNodeList,(void*)node);
				j++;
			}

			/* On vide  le buffer et on ferme le fichier*/
			fflush(file);
			fclose(file);
			return true;
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
