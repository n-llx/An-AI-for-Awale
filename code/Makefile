# Makefile

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11 -lm -g -O0

# Noms des fichiers source et objet
SRC = main.c jeu.c ia.c flat_mc.c mcts.c retrograde.c
OBJ = $(SRC:.c=.o)
HEADER = jeu.h ia.h flat_mc.h mcts.hb retrograde.h

# Nom de l'exécutable
EXE = exe

# Règle par défaut
all: $(EXE)

# Règle pour l'exécutable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Règle pour les fichiers objets
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJ) $(EXE)
