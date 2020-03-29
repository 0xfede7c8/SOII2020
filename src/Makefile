CC = gcc
CFLAGS = -I. -Wall -Werror -pedantic -Wextra -Wconversion -std=gnu11

CLIENT_DIR = client
SERVER_DIR = server

OUTDIR = build

CLIENT_OUT = $(OUTDIR)/$(CLIENT_DIR)

# %.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)

usb_writer: $(CLIENT_DIR)/usb_writer.c
	$(CC) -o $(OUTDIR)/$(CLIENT_DIR)/$@ $^ $(CFLAGS)
