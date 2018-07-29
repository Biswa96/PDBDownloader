#makefile for PEHeaderReader project

NAME=PDBDownloader
CC = gcc
CFLAGS=-Os -Wall -Werror -Wextra -Wundef
LIBS=-lWinInet
BIN=bin
SRC=src
OBJS= \
	$(BIN)\FileDownloader.obj \
	$(BIN)\PDBDownload.obj \
	$(BIN)\PEHeaderReader.obj

all: $(BIN) $(NAME)

$(BIN):
	mkdir $(BIN)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(BIN)\$@ $(LIBS)

$(BIN)\FileDownloader.obj: $(SRC)\FileDownloader.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)\PDBDownload.obj: $(SRC)\PDBDownload.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)\PEHeaderReader.obj: $(SRC)\PEHeaderReader.c
	$(CC) $(CFLAGS) -c $< -o $@

