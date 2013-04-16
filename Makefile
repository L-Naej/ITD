# $(BIN) est la nom du binaire genere
BIN = bin/itd
# FLAG
FLAGS = -g -O2 -Wall 
#Répertoire d'include des librairies
INCLUDES = include
# Répertoire des librairies
LIBDIR = ./lib
# Librairies
LIBS = -lglut -lGL -lGLU -lm -lSDL -lSDL_image

#MAC
LIBS_MAC  = -L/usr/X11R6/lib -lSDL -lGLU -lGL -lm -lX11
INCLUDES_MAC = -I/usr/X11R6/include
BIN_MAC = bin/itd-mac
ifeq ($(shell uname),OSX)
MAC_FLAGS = -D MAC
else
MAC_FLAGS = 
endif

# Compilateur
CC = gcc

SRC_PATH = src
OBJ_PATH = obj
BIN_PATH = bin

SRC_FILES = $(shell find $(SRC_PATH) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(SRC_FILES))

all : $(BIN) 

mac : $(BIN_MAC)
 
$(BIN): $(OBJ_FILES)
	@echo "PHASE DE LIEN..."
	$(CC) $(FLAGS) -L$(LIBDIR) -I$(INCLUDES) -o $(BIN) $^ $(LIBS)
	@echo "Compilation terminée.\nExécutez $(BIN) pour lancer le programme."

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
		@echo "Génération de $@ \c"
		@$(CC) -I$(INCLUDES) -I$(INCLUDES_MAC) $(MAC_FLAGS) $(FLAGS) -c $< -o $@
		@echo "=>OK"
		
$(BIN_MAC) : $(OBJ_FILES)
	@echo "PHASE DE LIEN..."
	$(CC) $(FLAGS) -I$(INCLUDES) -I$(INCLUDES_MAC) -o $(BIN) $^ $(LIBS_MAC)
	@echo "Compilation terminée.\nExécutez $(BIN) pour lancer le programme."
		
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

