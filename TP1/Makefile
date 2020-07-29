CC = gcc

SRC_DIR = src
SERVER_DIR = $(SRC_DIR)/server
CLIENT_DIR = $(SRC_DIR)/client

INCLUDE_DIR = $(SRC_DIR)/common
BIN_DIR = bin

CFLAGS = -O3 -ggdb -Wall -Werror -Wno-missing-field-initializers -pedantic -Wextra -Wconversion -std=gnu11 -I$(INCLUDE_DIR)

LIBS=-lm

CLIENT_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(CLIENT_DIR)/*.c))
SERVER_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(SERVER_DIR)/*.c))
AUTH_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(SERVER_DIR)/auth/*.c))
FILESERV_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(SERVER_DIR)/fileserv/*.c))
COMMON_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(INCLUDE_DIR)/*.c))

_AUTH_DEPS = csv.h user_authentication.h
AUTH_DEPS = $(patsubst %,$(SERVER_DIR)/auth/%,$(_AUTH_DEPS))

_CLIENT_DEPS = utils.h mbr.h md5.h
CLIENT_DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_CLIENT_DEPS))

.PHONY: clean all

all: client server auth fileserv

# Regla para el cliente
client: $(CLIENT_OBJECTS) $(COMMON_OBJECTS) $(CLIENT_DEPS)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS) -lssl -lcrypto

# Regla para el backend
server: $(SERVER_OBJECTS) $(COMMON_OBJECTS)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS)

# Regla para el auth service
auth: $(AUTH_OBJECTS) $(COMMON_OBJECTS) $(AUTH_DEPS)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS) -I$(SERVER_DIR)/auth
	cp $(SERVER_DIR)/auth/credentials.csv $(BIN_DIR)/

# Regla para el file server
fileserv: $(FILESERV_OBJECTS) $(COMMON_OBJECTS)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS) -lssl -lcrypto
	mkdir -p $(BIN_DIR)/files
	cp -r $(SERVER_DIR)/fileserv/files $(BIN_DIR)/

clean:
	-rm -rf $(CLIENT_DIR)/*.o $(SERVER_DIR)/*.o $(INCLUDE_DIR)/*.o $(BIN_DIR)/*
	-rm -rf $(SERVER_DIR)/auth/*.o $(SERVER_DIR)/fileserv/*.o 
