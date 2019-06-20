# Makefile for PDBDownloader project

NAME := PDBDownloader
BINDIR = ../bin
CFLAGS := -Os -std=c99 -m64 -Wall -Wextra
LIBS := -lWinInet
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.obj)
BINS := $(patsubst %.obj,$(BINDIR)/%.obj,$(OBJECTS))

EXECUTABLE=$(BINDIR)/$(NAME)

all: $(BINDIR) $(EXECUTABLE)

$(EXECUTABLE): $(BINS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(BINS): $(BINDIR)/%.obj: %.c
	$(CC) -c $(CFLAGS) $< -o $(BINDIR)/$*.obj

$(BINDIR):
	@mkdir ..\bin

clean:
	rm -rf $(BINDIR)

#END
