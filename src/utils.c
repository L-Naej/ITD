#include <dirent.h>
#include <sys/types.h>
#include "utils.h"
#include "graphics.h"
#define MAX_LENGHT 30
	
int readDirectory(char* map[]){
	char dataPath[] = "data/";
	DIR* dataRep = opendir(dataPath);
	if (dataRep ==NULL){
		perror("");
		exit(1);
	}

	struct dirent* fichierLu =NULL;
	
	int i=0;
	while((fichierLu = readdir(dataRep))!=NULL && i< NB_MAX_MAPS){
		if (fichierLu->d_name[0] != '.'){

			if (strlen(fichierLu->d_name)>  MAX_LENGHT)
				fprintf(stderr, "Erreur : le nom du fichier %s dépasse 30 caractères\n", fichierLu->d_name);
			else{
				map[i]=(char*)malloc(sizeof(char)*strlen(fichierLu->d_name));
				strcpy(map[i],fichierLu->d_name);

				i++;
			}
		}
	}

	int nb_cartes = i;

	if (closedir(dataRep)!=0){
		printf("erreur dans la fermeture du repertoire\n");
	}
	return nb_cartes;
}
