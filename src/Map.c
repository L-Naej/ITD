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
	
	newMap.nodeList = createEmptyList(NODE);
	
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
	while( (tmpNode = (Point3D*) nextData(pathNodeList) ) != NULL && !pointIsNode){
		pointIsNode = arePointsEquals(currentNode, *tmpNode);
	}
	
	if(tmpNode == NULL){
		fprintf(stderr, "Erreur fatale, le point passé en paramètre de nextNode n'est pas un noeud valide.\n");
		exit(-1);
	}
	
	//Puis on retourne celui qui le suit.
	//La liste gère toute seule si elle est en fin de liste et ne "dépasse" pas (reste sur le dernier node)
	tmpNode = (Point3D*) nextData(pathNodeList);

	return *tmpNode;
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
	
	printf("\nNode list (%d nodes)\n", map.nodeList->size);
	goToHeadList(map.nodeList);
	
	Point3D* cur = NULL;
	while( (cur = nextData(map.nodeList)) != NULL){
		printf("Node %d : x=%f y=%f\n", map.nodeList->position, cur->x, cur->y);
	}
}
