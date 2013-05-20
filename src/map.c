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
	int nbItemsLus = 0;
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
	fscanf(file,"%s30 \n",map->name);


	/*rajouter width et height quand on aura le fichier .ppm*/

	int R,V,B;	

	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"chemin")!= 0){
		printf("mot-clé 'chemin' incorrect\n");
		return -1;
	}
	memset (keyword,0,sizeof(keyword));

	nbItemsLus = fscanf(file,"%d %d %d\n",&R,&V,&B);
	if(nbItemsLus < 3){
		fprintf(stderr, "Map %s : couleur du chemin mal définie\n", map->name);
		return -1;
	}
	if( R < 0 || R > 255 || V < 0 || V > 255 || B < 0 || B > 255){
		fprintf(stderr,"Map %s : couleur du chemin invalide ([%d,%d,%d])\n", map->name, R,V,B);
		return -1;
	}
	map->pathColor.red =(unsigned char)R;
	map->pathColor.green = (unsigned char)V;
	map->pathColor.blue =(unsigned char)B;

	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"noeud")!= 0){
		printf("mot-clé 'noeud' incorrect\n");
		return -1;
	}
	memset (keyword,0,sizeof(keyword));

	nbItemsLus = fscanf(file,"%d %d %d\n",&R,&V,&B);
	if(nbItemsLus < 3){
		fprintf(stderr, "Map %s : couleur des noeuds du chemin mal définie\n", map->name);
		return -1;
	}
	if( R < 0 || R > 255 || V < 0 || V > 255 || B < 0 || B > 255){
		fprintf(stderr,"Map %s : couleur des noeuds du chemin invalide ([%d,%d,%d])\n", map->name, R,V,B);
		return -1;
	}
	map->nodeColor.red = (unsigned char)R;
	map->nodeColor.green = (unsigned char)V;
	map->nodeColor.blue = (unsigned char)B;
	
	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"construct")!= 0){
		printf("mot-clé 'construct' incorrect\n");
		return -1;
	}
	memset (keyword,0,sizeof(keyword));

	nbItemsLus = fscanf(file,"%d %d %d\n",&R,&V,&B);
	if(nbItemsLus < 3){
		fprintf(stderr, "Map %s : couleur des zones de construction mal définie\n", map->name);
		return -1;
	}
	if( R < 0 || R > 255 || V < 0 || V > 255 || B < 0 || B > 255){
		fprintf(stderr,"Map %s : couleur des zones de construction invalide ([%d,%d,%d])\n", map->name, R,V,B);
		return -1;
	}
	map->constructAreaColor.red = (unsigned char)R;
	map->constructAreaColor.green = (unsigned char)V;
	map->constructAreaColor.blue = (unsigned char)B;

	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"in")!= 0){
		printf("mot-clé'in' incorrect\n");
		return -1;
	} 
	memset (keyword,0,sizeof(keyword));

	nbItemsLus = fscanf(file,"%d %d %d\n",&R,&V,&B);
	if(nbItemsLus < 3){
		fprintf(stderr, "Map %s : couleur de la zone d'entrée mal définie\n", map->name);
		return -1;
	}
	if( R < 0 || R > 255 || V < 0 || V > 255 || B < 0 || B > 255){
		fprintf(stderr,"Map %s : couleur de la zone d'entrée invalide ([%d,%d,%d])\n", map->name, R,V,B);
		return -1;
	}
	map->inAreaColor.red = (unsigned char)R;
	map->inAreaColor.green = (unsigned char)V;
	map->inAreaColor.blue = (unsigned char)B;

	fscanf(file,"%s \n",keyword);
	if (strcmp(keyword,"out")!= 0){
		printf("mot-clé'out' incorrect\n");
		return -1;
	} 
	memset (keyword,0,sizeof(keyword));

	nbItemsLus = fscanf(file,"%d %d %d\n",&R,&V,&B);
	if(nbItemsLus < 3){
		fprintf(stderr, "Map %s : couleur du chemin mal définie\n", map->name);
		return -1;
	}
	if( R < 0 || R > 255 || V < 0 || V > 255 || B < 0 || B > 255){
		fprintf(stderr,"Map %s : couleur du chemin invalide ([%d,%d,%d])\n", map->name, R,V,B);
		return -1;
	}
	map->outAreaColor.red = (unsigned char)R;
	map->outAreaColor.green = (unsigned char)V;
	map->outAreaColor.blue = (unsigned char)B;
	
	int size;
	fscanf(file,"%d\n",&size);

	//Chargement de l'image pour connaître les dimensions
	if(! loadPpmMap(map) ){
		return false;
	}

	

	Point3D* node1 = (Point3D*)malloc (sizeof(Point3D)); 
	nbItemsLus = fscanf(file,"%f %f\n",&(node1->x),&(node1->y));
	if(nbItemsLus < 2){
		fprintf(stderr, "Map %s : nombre de coordonnées incorrecte\n", map->name);
		return -1;
	}
	if(node1->x >= map->width || node1->x < 0
		|| node1->y >= map->height || node1->y < 0){
		fprintf(stderr, "Map %s : coordonnées de chemin en dehors de la carte(%f, %f)\n", map->name, node1->x, node1->y);
		return -1;
	}
	node1->z = 0.0;

	map->pathNodeList = createList((void*)node1); 

	int j=0;
	
	while (j<size-1){
		if(feof(file)){
			fprintf(stderr, "Map %s : nombre de noeuds incorrect\n", map->name);
			return 0;
		}
		Point3D* node = (Point3D*)malloc (sizeof(Point3D));		
		nbItemsLus = fscanf(file,"%f %f\n",&(node->x),&(node->y));
		if(nbItemsLus < 2){
			fprintf(stderr, "Map %s : nombre de coordonnées incorrect\n", map->name);
			return 0;
		}
		if(node->x >= map->width || node->x < 0
		|| node->y >= map->height || node->y < 0){
			fprintf(stderr, "Map %s : coordonnées de chemin en dehors de la carte (%f, %f)\n", map->name, node->x, node->y);
			return -1;
		}
		node->z = 0.0;
		insertBottomCell(map->pathNodeList,(void*)node);
		j++;
	}


	
			

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
				if (!feof(file)){
					fprintf(stderr,"Il y a trop de coordonnées par rapport au nombres de noeuds annoncé \n");
					return 0;
				}	
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
			printf("mot-clé'powerR' incorrect ou nombres de coordonnées de noeuds incorrect\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->powerR)) < 1){
			fprintf(stderr, "Map %s : puissance tours rocket incorrecte\n", map->name);
			return false;	
		}

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rateR")!= 0){
			printf("mot-clé'rateR' incorrect (%s à la place)\n", keyword);;
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->rateR)) < 1){ 
			fprintf(stderr, "Map %s : cadence tours rocket incorrecte\n", map->name);
			return false;	
		}

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rangeR")!= 0){
			printf("mot-clé'rangeR' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->rangeR)) < 1){
			fprintf(stderr, "Map %s : portée tours rocket incorrecte\n", map->name);
			return false;	
		}

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"costR")!= 0){
			printf("mot-clé'costR' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->costR)) <1){
			fprintf(stderr, "Map %s : coût tours rocket incorrecte\n", map->name);
			return false;	
		}
	/*          données des tours LASER         */

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"powerL")!= 0){
			printf("mot-clé'powerL' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->powerL))<1){
			fprintf(stderr, "Map %s : puissance tours laser incorrecte\n", map->name);
			return false;	
		}

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rateL")!= 0){
			printf("mot-clé'rateL' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->rateL))<1){
			fprintf(stderr, "Map %s : cadence tours laser incorrecte\n", map->name);
			return false;	
		}
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rangeL")!= 0){
			printf("mot-clé'rangeL' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->rangeL))<1){
			fprintf(stderr, "Map %s : cadence tours laser incorrecte\n", map->name);
			return false;	
		}
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"costL")!= 0){
			printf("mot-clé'costL' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->costL))<1){
			fprintf(stderr, "Map %s : coût tours laser incorrecte\n", map->name);
			return false;	
		}

	/*          données des tours MITRAILLETTE         */
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"powerM")!= 0){
			printf("mot-clé'powerM' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->powerM))<1){
			fprintf(stderr, "Map %s : puissance tours gun incorrecte\n", map->name);
			return false;	
		}
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rateM")!= 0){
			printf("mot-clé'rateM' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->rateM))<1){
			fprintf(stderr, "Map %s : cadence tours gun incorrecte\n", map->name);
			return false;	
		}
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rangeM")!= 0){
			printf("mot-clé'rangeM' incorrect (%s à la place)\n", keyword);
			return -1;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->rangeM))<1){
			fprintf(stderr, "Map %s : portée tours gun incorrecte\n", map->name);
			return false;	
		}
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"costM")!= 0){
			printf("mot-clé'costM' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->costM))<1){
			fprintf(stderr, "Map %s : coût tours gun incorrecte\n", map->name);
			return false;	
		}
	/*          données des tours HYBRID         */

		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"powerH")!= 0){
			printf("mot-clé'powerH' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->powerH))<1){
			fprintf(stderr, "Map %s : puissance tours hybrides incorrecte\n", map->name);
			return false;	
		}
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rateH")!= 0){
			printf("mot-clé'rateH' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->rateH))<1){
			fprintf(stderr, "Map %s : cadence tours hybrides incorrecte\n", map->name);
			return false;	
		}
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"rangeH")!= 0){
			printf("mot-clé'rangeH' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->rangeH))<1){
			fprintf(stderr, "Map %s : portée tours hybrides incorrecte\n", map->name);
			return false;	
		}
		fscanf(file,"%s \n",keyword);
		if (strcmp(keyword,"costH")!= 0){
			printf("mot-clé'costH' incorrect (%s à la place)\n", keyword);
			return false;
		} 
		memset (keyword,0,sizeof(keyword));
		if(fscanf(file,"%d\n",&(map->towerdatas->costH))<1){
			fprintf(stderr, "Map %s : coût tours hybrides incorrecte\n", map->name);
			return false;	
		}

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
			if(colorPixel.red == 179 && colorPixel.green == 73 && colorPixel.blue == 155) {			
				
				colorPixel.red = map->constructAreaColor.red;
				colorPixel.green = map->constructAreaColor.green;
				colorPixel.blue = map->constructAreaColor.blue;	

				map->tabXYConstruct[i][j] = true;
				
				newColorPixel=SDL_MapRGB(map->image->format, colorPixel.red, colorPixel.green, colorPixel.blue);
				modifColorPixel(map->image, i, j, newColorPixel);
			}
			else map->tabXYConstruct[i][j] = false;
		}
	}
	
	return true;
}

void cleanMap(Map* map){
	if(map == NULL) return;
	free(map->name);
	map->name = NULL;
	if(map->pathNodeList != NULL)
		freeListComplete(map->pathNodeList);
	map->pathNodeList = NULL;
	
	SDL_FreeSurface(map->image);
	map->image = NULL;
	int i = 0;
	if(map->tabXYConstruct != NULL){
		for(i = 0; i < map->width; i++){
			free(map->tabXYConstruct[i]);
		}
	}
	map->tabXYConstruct = NULL;
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
