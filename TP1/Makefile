CC = gcc

SRC_DIR = src
SERVER_DIR = $(SRC_DIR)/server
CLIENT_DIR = $(SRC_DIR)/client

INCLUDE_DIR = $(SRC_DIR)
BIN_DIR = bin

OBJ_CLIENT_DIR = $(BIN_DIR)/obj/client
OBJ_SERVER_DIR = $(BIN_DIR)/obj/client

CLIENT_OUT = $(BIN_DIR)
SERVER_OUT = $(BIN_DIR)

CFLAGS = -Wall -Werror -pedantic -Wextra -Wconversion -std=gnu11 -I$(INCLUDE_DIR)

LIBS=-lm

CLIENT_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(CLIENT_DIR)/*.c))
SERVER_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(SERVER_DIR)/*.c))
COMMON_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/*.c))

.PHONY: clean all

# Regla para el cliente
client: $(CLIENT_OBJECTS)
	$(CC) -o $(CLIENT_OUT)/$@ $^ $(COMMON_OBJECTS) $(CFLAGS)

# Regla para el backend
server: $(SERVER_OBJECTS)
	$(CC) -o $(SERVER_OUT)/$@ $^ $(COMMON_OBJECTS) $(CFLAGS)

clean:
	rm -rf $(OBJECTS_DIR)/*.o $(CLIENT_OUT)/* $(SERVER_OUT)/*