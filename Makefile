CC=gcc
CFLAGS=-Iinclude -Wall -Werror -g -Wno-unused

SSRC=$(shell find src -name '*.c')
DEPS=$(shell find include -name '*.h')
DFLAGS := -g -DDEBUG

LIBS=-lpthread

all: setup server 

debug:
	$(CC) $(DFLAGS) $(CFLAGS) $(SSRC) lib/protocol.o -o bin/zotReg_server $(LIBS)
setup:
	mkdir -p bin 
	cp lib/zotReg_client bin/zotReg_client

server: setup
	$(CC) $(CFLAGS) $(SSRC) lib/protocol.o -o bin/zotReg_server $(LIBS)
	
.PHONY: clean

clean:
	rm -rf bin 
