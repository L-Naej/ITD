#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"
#include "tower.h"
#include <SDL/SDL_image.h>

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
	
	newMap.towerdatas = createEmptyTowerStat();
		if(newMap.towerdatas == NULL){
		perror("Erreur fatale : Impossible d'allouer la mémoire pour stocker les données des tours.\n");
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
	ListState* state = saveListState(pathNodeList);
	
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
	
	restoreListState(state);
	free(state);
	//Si on est sur le dernier point on le retourne lui même
	if(tmpNode == NULL) return currentNode;

	return *tmpNode;
}

Point3D getStartPoint(const Map* map){
	if(map == NULL) return PointXYZ(-1,-1,-1);
	
	ListState* state = saveListState(map->pathNodeList);
	
	goToHeadList(map->pathNodeList);
	Point3D* tmpPtr = (Point3D*) nextData(map->pathNodeList);
	if(tmpPtr == NULL){
		printf("startPoint NULL\n");
		exit(-1);
	}
	Point3D startPoint = *tmpPtr;
	
	restoreListState(state);
	free(state);
	return startPoint;
}

Point3D getEndPoint(const Map* map){
	if(map == NULL) return PointXYZ(-1,-1,-1);
	ListState* state = saveListState(map->pathNodeList);
	
	goToBottomCell(map->pathNodeList);
	Point3D* tmpPtr = (Point3D*) currentData(map->pathNodeList);
	if(tmpPtr == NULL){
		printf("endPoint NULL\n");
		exit(-1);
	}
	Point3D endPoint = *tmpPtr;
	
	restoreListState(state);
	free(state);
	return endPoint;
}

int testItdValid(int R,int V,int B){
	if ((R>=0 && R<= 255) && (V>=0 && V<= 255) && (B>=0 && B<= 255)){
		return 1;
	}
	else{
		return 0;
	}

}

int loadITD1 (Map* map, FILE* file, char* keyword){
	/* nom de l'image*/

	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"carte")!= 0){
		printf("mot-clé 'carte' incorrect\n");
		return -1;
	}
	memset (keyword,0,sizeof(keyword));

	(map->name) = (char*)malloc(sizeof(char)*MAX_LENGHT);
	if (map->name ==NULL){
		printf("erreur d'allocation du nom de la map");
		return -1;
	}
 	/*  on a dit que le nom de la carte ferait 30 caractères maxi*/
	fscanf(file,"%s \n",map->name);


	/*rajouter width et height quand on aura le fichier .ppm*/

	int R,V,B;
			

	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"chemin")!= 0){
		printf("mot-clé 'chemin' incorrect\n");
		return -1;
	}
	memset (keyword,0,sizeof(keyword));

	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->pathColor.red =(unsigned char)R;
	map->pathColor.green = (unsigned char)V;
	map->pathColor.blue =(unsigned char)B;
	if (testItdValid(map->pathColor.red,map->pathColor.green,map->pathColor.blue)!= 1){
		printf("ligne de couleur de chemin erronée \n");
		return 0;
	}



	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"noeud")!= 0){
		printf("mot-clé 'noeud' incorrect\n");
		return -1;
	}
	memset (keyword,0,sizeof(keyword));

	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->nodeColor.red = (unsigned char)R;
	map->nodeColor.green = (unsigned char)V;
	map->nodeColor.blue = (unsigned char)B;
	if (testItdValid(map->nodeColor.red,map->nodeColor.green,map->nodeColor.blue)!= 1){
		printf("ligne de couleur de noeud erronée \n");
		return 0;
	}



	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"construct")!= 0){
		printf("mot-clé 'construct' incorrect\n");
		return -1;
	}
	memset (keyword,0,sizeof(keyword));

	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->constructAreaColor.red = (unsigned char)R;
	map->constructAreaColor.green = (unsigned char)V;
	map->constructAreaColor.blue = (unsigned char)B;
	if (testItdValid(map->constructAreaColor.red,map->constructAreaColor.green,map->constructAreaColor.blue)!= 1){
		printf("ligne de couleur de zone constructible erronée \n");
		return 0;
	}



	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"in")!= 0){
		printf("mot-clé'in' incorrect\n");
		return -1;
	} 
	memset (keyword,0,sizeof(keyword));

	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->inAreaColor.red = (unsigned char)R;
	map->inAreaColor.green = (unsigned char)V;
	map->inAreaColor.blue = (unsigned char)B;
	if (testItdValid(map->inAreaColor.red,map->inAreaColor.green,map->inAreaColor.blue)!= 1){
		printf("ligne de couleur d'entrée erronée \n");
		return 0;
	}



	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"out")!= 0){
		printf("mot-clé'out' incorrect\n");
		return -1;
	} 
	memset (keyword,0,sizeof(keyword));

	fscanf(file,"%d %d %d\n",&R,&V,&B);
	map->outAreaColor.red = (unsigned char)R;
	map->outAreaColor.green = (unsigned char)V;
	map->outAreaColor.blue = (unsigned char)B;
	if (testItdValid(map->outAreaColor.red,map->outAreaColor.green,map->outAreaColor.blue)!= 1){
		printf("ligne de couleur de sortie erronée \n");
		return 0;
	}

	int size;
	fscanf(file,"%d\n",&size);



	Point3D* node1 = (Point3D*)malloc (sizeof(Point3D)); 
	fscanf(file,"%f %f\n",&(node1->x),&(node1->y));
	node1->z = 0.0;

	map->pathNodeList = createList((void*)node1); 

	int j=0;


	while (j<size-1){

		Point3D* node = (Point3D*)malloc (sizeof(Point3D));		
		fscanf(file,"%f %f\n",&(node->x),&(node->y));
		node->z = 0.0;
		if ((node->x)==0 && (node->y)==0){
			printf("nombre de coordonnée de noeuds incorrect - error 1- \n");
			return 0;
		}
		insertBottomCell(map->pathNodeList,(void*)node);
		j++;
	}

	
	loadPpmMap(map);
	
	transformCoordToOpenGL(map);

	return 1;
} 

void transformCoordToOpenGL(Map* map){
	if(map == NULL) return;
	goToHeadList(map->pathNodeList);
	Point3D* cur = NULL;
	
	while( (cur = (Point3D*) nextData(map->pathNodeList) ) != NULL){
		*cur = itdToOpenGL(map->width, map->height, *cur);
	}
}

bool loadMap(Map* map, const char* pathToItdFile){	

	/* On ouvre le fichier */
	FILE* file;
	char* keyword = (char*) malloc (sizeof(char)*11);

	file = fopen(pathToItdFile,"r");
	if(file == NULL) return false;
	
	char versionMap [MAX_LENGHT];
	int k;
	for (k=0; k<MAX_LENGHT;k++){
		versionMap[k]= 0;
	}
	int i;
	for (i=0; i< 6; i++){
		fscanf(file,"%c",versionMap+i);
	}
	
	if (strcmp(versionMap,"@ITD 1")!= 0 && strcmp(versionMap,"@ITD 2")!=0 ){
		fprintf(stderr, "Fichier incompatible\n");
		return false;
	}
	
	if (strcmp(versionMap,"@ITD 1")==0){

		if (loadITD1(map,file,keyword)==1){
			/* On vide  le buffer et on ferme le fichier*/
			fflush(file);
			fclose(file);
			printf("Carte chargée\n");
			return true;
		}
		return false;
	}
	
	
	if (strcmp(versionMap,"@ITD 2")==0){

		if( loadITD1(map,file,keyword) !=1 ) return false;
		
	/*          données des tours ROCKETS         */
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"powerR")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->powerR));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rateR")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->rateR));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rangeR")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->rangeR));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"costR")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->costR));

	/*          données des tours LASER         */

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"powerL")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->powerL));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rateL")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->rateL));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rangeL")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->rangeL));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"costL")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->costL));

	/*          données des tours MITRAILLETTE         */
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"powerM")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->powerM));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rateM")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->rateM));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rangeM")!= 0){
			printf("mot-clé'in' incorrect\n");
			return -1;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->rangeM));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"costM")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->costM));

	/*          données des tours HYBRID         */

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"powerH")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->powerH));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rateH")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->rateH));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rangeH")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->rangeH));

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"costH")!= 0){
			printf("mot-clé'in' incorrect\n");
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		fscanf(file,"%d\n",&(map->towerdatas->costH));


		fflush(file);
		fclose(file);
		printf("Carte chargée\n");
		return true;
	}
	return true;
}

			/* Récupération de la couleur d'un pixel */
Uint32 recupColorPixel(SDL_Surface *surface, int x, int y){

    /*nbOctetsParPixel = nombre d'octets utilisés pour stocker un pixel.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel)
    {
        case 1:
            return *p;
 
        case 2:
            return *(Uint16 *)p;
 
        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
 
        case 4:
            return *(Uint32 *)p;
 
        default:
            return 0;
    }
}


/* Modification de la couleur du pixel */
void modifColorPixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
   
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
 
    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;
 
        case 2:
            *(Uint16 *)p = pixel;
            break;
 
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
 
        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

	
	

/*Chargement de l'image ppm*/
bool loadPpmMap(Map* map){
	
	char chemin [38] = "images/";
	strcat(chemin,map->name);
  	map->image = IMG_Load(chemin);
  	
  	if(map->image == NULL) {
   		fprintf(stderr, "Impossible de charger le fichier %s.\n", map->name);
  		 return false;
	}	  
	
	int i,j = 0;
	Color3u colorPixel;
	Uint32 initColorPixel;
	Uint32 newColorPixel;
	int nbPixels = map->image->w * map->image->h;
	int nbLignes = map->image->w;
	int nbCol = map->image->h;
	map->nbPixels = nbPixels;
	map->width = map->image->w;
	map->height = map->image->h;
 
	map->tabXYConstruct = (bool**) malloc(nbLignes*sizeof(bool*));

	if(map->tabXYConstruct == NULL){
		printf("Erreur lors de l'allocation du tableau de mémorisation des zones constructibles \n");
           	exit(EXIT_FAILURE);
	} 
	for(i=0; i<map->image->w; i++) {	
	    map->tabXYConstruct[i] = (bool*) malloc(nbCol*sizeof(bool));	
		for(j=0; j<map->image->h; j++) {
			initColorPixel = recupColorPixel(map->image, i, j);
			SDL_GetRGB(initColorPixel, map->image->format, &(colorPixel.red), &(colorPixel.green),&(colorPixel.blue));
			if(colorPixel.red == 120 && colorPixel.green == 120 && colorPixel.blue == 120) {				
				colorPixel.red = map->constructAreaColor.red;
				colorPixel.green = map->constructAreaColor.green;
				colorPixel.blue = map->constructAreaColor.blue;	
				map->tabXYConstruct[i][j] = true;
				
				newColorPixel=SDL_MapRGB(map->image->format, colorPixel.red, colorPixel.green, colorPixel.blue);
				modifColorPixel(map->image, i, j, newColorPixel);
			} else if(colorPixel.red == 223 && colorPixel.green == 11 && colorPixel.blue == 216) {			
				
				colorPixel.red = map->pathColor.red;
				colorPixel.green = map->pathColor.green;
				colorPixel.blue = map->pathColor.blue;	

				map->tabXYConstruct[i][j] = false;
				
				newColorPixel=SDL_MapRGB(map->image->format, colorPixel.red, colorPixel.green, colorPixel.blue);
				modifColorPixel(map->image, i, j, newColorPixel);
			}
		}
	}
	
	return true;
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
