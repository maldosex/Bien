CC=gcc
CFLAGS=-Wall -Wextra

BIN=bin
SRC=src

all: $(BIN)/cliente $(BIN)/servidor

$(BIN)/cliente:
	mkdir -p $(BIN)
	$(CC) cliente.c \
	      api.c \
	      gui.c \
	      cJSON.c \
	      modelo.c \
	      -lncurses -lmenu -lform -pthread \
	      -o $(BIN)/cliente

$(BIN)/servidor:
	mkdir -p $(BIN)
	$(CC) servidor.c \
	      db.c \
	      modelo.c \
	      cJSON.c \
	      -pthread \
	      -o $(BIN)/servidor

# Objetivo para limpiar los archivos generados
clean:
	rm -rf $(BIN)

# PHONY evita conflictos si existen archivos llamados 'all' o 'clean'
.PHONY: all clean