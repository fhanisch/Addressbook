all: Addressbook Server

Addressbook: main.c
	gcc -g -o Addressbook main.c `pkg-config gtk+-3.0 --cflags --libs`

Server: server.c
	gcc -o Server server.c
