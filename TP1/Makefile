CC = gcc

SRC_DIR = src
SERVER_DIR = $(SRC_DIR)/server
CLIENT_DIR = $(SRC_DIR)/client

INCLUDE_DIR = $(SRC_DIR)/common
BIN_DIR = bin

CFLAGS = -O3 -Wall -Werror -pedantic -Wextra -Wconversion -std=gnu11 -I$(INCLUDE_DIR)

LIBS=-lm

CLIENT_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(CLIENT_DIR)/*.c))
SERVER_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(SERVER_DIR)/*.c))
AUTH_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(SERVER_DIR)/auth/*.c))
FILESERV_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(SERVER_DIR)/fileserv/*.c))
COMMON_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(INCLUDE_DIR)/*.c))

.PHONY: clean all

all: client server auth fileserv

# Regla para el cliente
client: $(CLIENT_OBJECTS) $(COMMON_OBJECTS)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS)

# Regla para el backend
server: $(SERVER_OBJECTS) $(COMMON_OBJECTS)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS)

# Regla para el auth service
auth: $(AUTH_OBJECTS) $(COMMON_OBJECTS)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS)

# Regla para el file server
fileserv: $(FILESERV_OBJECTS) $(COMMON_OBJECTS)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS)

clean:
	rm -rf $(CLIENT_DIR)/*.o $(SERVER_DIR)/*.o $(INCLUDE_DIR)/*.o $(BIN_DIR)/*	 