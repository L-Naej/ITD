# $(BIN) est la nom du binaire genere
BIN = bin/itd
# FLAG
FLAGS = -g -03 -Wall 
#Répertoire d'include des librairies
INCLUDES = include
# Répertoire des librairies
LIBDIR = ./lib
# Librairies
LIBS = -lglut -lGL -lGLU -lm -lSDL -lSDL_image -lSDL_ttf

#MAC
ifeq ($(shell uname),Darwin)
LIBS  = -L/usr/X11R6/lib -L/opt/local/lib -lSDL -lSDLmain -lSDL_image -lSDL_ttf -framework Cocoa -framework GLUT -framework OpenGL -lm -lX11
INCLUDES := $(INCLUDES) -I/usr/X11R6/include -I/opt/local/include -D_GNU_SOURCE=1 -D_THREAD_SAFE
BIN_MAC = bin/itd-mac 
FLAGS := $(FLAGS) -D MAC #SDLmain.m
endif

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
		
$(BIN_MAC) : $(OBJ_FILES)
	@echo "PHASE DE LIEN..."
	$(CC) $(FLAGS) -I$(INCLUDES) -o $(BIN) $^ $(LIBS)
	@echo "Compilation terminée.\nExécutez $(BIN) pour lancer le programme."
		
	
clean:
	rm -f $(BIN)
	rm -f $(OBJ_PATH)/*.o $(SRC_PATH)/*~ 
	find . -name "*~" -exec rm {} \;

#Nettoie aussi dans les libs
bigclean: clean
	@(cd $(LIBDIR) && $(MAKE) bigclean)

