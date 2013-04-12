# $(BIN) est la nom du binaire genere
BIN = bin/itd
# FLAG
FLAGS = -g -Wall $(shell sdl-config --cflags)
#Répertoire d'include des librairies
INCLUDES = include
# Répertoire des librairies
LIBDIR = ./lib
# Librairies
LIBS = -lglut -lGL -lGLU -lm $(shell sdl-config --libs) -lSDL_image
# Compilateur
CC = gcc

SRC_PATH = src
OBJ_PATH = obj
BIN_PATH = bin

SRC_FILES = $(shell find $(SRC_PATH) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(SRC_FILES))

all : $(BIN) 
 
$(BIN): $(OBJ_FILES)
	@echo "PHASE DE LIEN..."
	$(CC) $(FLAGS) -L$(LIBDIR) -I$(INCLUDES) -o $(BIN) $^ $(LIBS)
	@echo "Compilation terminée.\nExécutez $(BIN) pour lancer le programme."

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
		@echo "Génération de $@ \c"
		@$(CC) -I$(INCLUDES) $(FLAGS) -c $< -o $@
		@echo "=>OK"
		
$(LIBS):
	@echo "\n-----Compilation des librairies du projet-----"
	@(cd $(LIBDIR) && $(MAKE))
	@echo "-----Fin de la compilation des librairies.-----\n"
	
clean:
	rm -f $(BIN)
	rm -f $(OBJ_PATH)/*.o $(SRC_PATH)/*~ $(INCLUDES)/*~ 
	find . -name "*~" -exec rm {} \;

#Nettoie aussi dans les libs
bigclean: clean
	@(cd $(LIBDIR) && $(MAKE) bigclean)

