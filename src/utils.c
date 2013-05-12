#include <dirent.h>
#include <sys/types.h>
#include "utils.h"
#define MAX_LENGHT 30
	
int readDirectory(char* map[],char* rootPath){
	char* dataPath = (char*)malloc(sizeof(char)*(strlen(rootPath)+5));
	strcpy(dataPath,rootPath);
	dataPath = strcat(dataPath,"data");
	DIR* dataRep = opendir(dataPath);
	if (dataRep ==NULL){
		perror("");
		exit(1);
	}

	struct dirent* fichierLu =NULL;
	
	int i=1;


	while((fichierLu = readdir(dataRep))!=NULL && i<= 5){
		if (strcmp(fichierLu->d_name,".")!=0 && strcmp(fichierLu->d_name,"..")!=0){

			if (strlen(fichierLu->d_name)>  MAX_LENGHT)
				printf("Désolé, le nom de la map dépasse 30 caractères\n");
			else{
				map[i]=(char*)malloc(sizeof(char)*strlen(fichierLu->d_name));
				strcpy(map[i],fichierLu->d_name);

				i++;
			}
		}
	}

	int nb_cartes = i-1;

	if (closedir(dataRep)!=0){
		printf("erreur dans la fermeture du repertoire\n");
	}
	return nb_cartes;
}
