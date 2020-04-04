CC = gcc

SERVER_DIR = server
CLIENT_DIR = client

INCLUDE_DIR = common
OUTDIR = ../build
OBJECTS_DIR = $(OUTDIR)/obj

CLIENT_OUT = $(OUTDIR)/$(CLIENT_DIR)
SERVER_OUT = $(OUTDIR)/$(SERVER_DIR)

CFLAGS = -Wall -Werror -pedantic -Wextra -Wconversion -std=gnu11 -I$(INCLUDE_DIR)

LIBS=-lm

# Dependencias para cada proyecto
_COMMON_DEPS = tcp_client.h
COMMON_DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_CLIENT_DEPS = 
CLIENT_DEPS = $(patsubst %,$(CLIENT_DIR)/%,$(_DEPS))

_BACKEND_DEPS = 
BACKEND_DEPS = $(patsubst %,$(CLIENT_DIR)/%,$(_DEPS)) 

# Reglas para buildear los objetos de cada proyecto
$(OBJECTS_DIR)/%.o: $(INCLUDE_DIR)/%.c $(COMMON_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJECTS_DIR)/%.o: $(CLIENT_DIR)/%.c $(CLIENT_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJECTS_DIR)/%.o: $(SERVER_DIR)/%.c $(BACKEND_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Dependencias de objetos de cada proyecto
_OBJ_CLIENT = usb_writer.o tcp_client.o 
OBJ_CLIENT = $(patsubst %,$(OBJECTS_DIR)/%,$(_OBJ_CLIENT))

_OBJ_SERVER =  usb_writer_backend.o
OBJ_SERVER = $(patsubst %,$(OBJECTS_DIR)/%,$(_OBJ_SERVER))


.PHONY: clean

# Rule para el cliente
usb_writer: $(OBJ_CLIENT)
	$(CC) -o $(CLIENT_OUT)/$@ $^ $(CFLAGS)


#Rule para el server
usb_writer_backend: $(OBJ_SERVER)
	$(CC) -o $(SERVER_OUT)/$@ $^ $(CFLAGS)

clean:
	rm -rf $(OBJECTS_DIR)/*.o $(CLIENT_OUT)/* $(SERVER_OUT)